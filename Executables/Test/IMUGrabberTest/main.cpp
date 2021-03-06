/** @file */

#include <chrono>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <serial/serial.h>
#include <vtkActor.h>
#include <vtkAxes.h>
#include <vtkCellArray.h>
#include <vtkCommand.h>
#include <vtkCubeSource.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

#include <YukariCommon/LoggingService.h>
#include <YukariIMU/IMUGrabberFactory.h>
#include <YukariMSP/MSPClient.h>
#include <YukariMSP/MSPParsers.h>

#include "VTKIMUActorCallback.h"
#include "VTKIMUCalibrationInteractionStyle.h"

using namespace Yukari::Common;
using namespace Yukari::IMU;
using namespace Yukari::Maths;
using namespace Yukari::MSP;
using namespace Yukari::IMUGrabberTest;
namespace po = boost::program_options;

int runGrabberVisualisation(IIMUGrabber::Ptr grabber, float cubeSize,
                            const boost::filesystem::path &dataFile);

int main(int argc, char **argv)
{
  auto logger = LoggingService::Instance().getLogger("main");

  /* Init command line */
  po::options_description desc("Allowed options");
  po::variables_map args;

  // clang-format off
  desc.add_options()
    ("help", "Show brief usage message")
    ("loglevel", po::value<std::string>()->default_value("info"), "Global log level")
    ("grabber", po::value<std::string>()->default_value("dummy"), "IMU grabber type")
    ("orientation", po::value<std::string>()->default_value("[0, 0, 0] 0"), "Relative IMU orientation as \"[axis] angle\"")
    ("position", po::value<std::string>()->default_value("[0, 0, 0]"), "Relative IMU position as \"[position]\"")
    ("cubesize" ,po::value<float>()->default_value(1.0f), "Width and depth of visualisation cube")
    ("datafile", po::value<std::string>(), "File to save recorded data to");
  // clang-format on

  /* Parse command line args */
  try
  {
    po::store(po::parse_command_line(argc, argv, desc), args);
  }
  catch (po::error const &e)
  {
    logger->critical("{}", e.what());
    return 1;
  }

  /* Show usage */
  if (args.count("help"))
  {
    std::cout << desc << "\n";
    return 1;
  }

  /* Configure logging */
  LoggingService::Instance().configure(args);

  /* Get data file name */
  boost::filesystem::path dataFile;
  if (args.count("datafile"))
    dataFile = args["datafile"].as<std::string>();

  if (dataFile.empty())
    logger->info("No data file will be produced.");
  else
    logger->info("Got data file: {}", dataFile);

  /* List all ports */
  std::vector<serial::PortInfo> ports = serial::list_ports();
  for (auto it = ports.begin(); it != ports.end(); ++it)
    logger->info("({}, {}, {})", it->port, it->description, it->hardware_id);

  /* Create IMU grabber */
  auto grabber = IMUGrabberFactory::Create(args["grabber"].as<std::string>());
  if (!grabber)
  {
    logger->critical("Failed to create IMU grabber");
    return 1;
  }

  /* Set relative IMU transform from command line args */
  grabber->setTransform(Transform(args));

  /* Run visualisation */
  runGrabberVisualisation(grabber, args["cubesize"].as<float>(), dataFile);

  logger->info("Exiting.");
  return 0;
}

vtkPolyData *generateCube(Eigen::Vector3f d)
{
  d /= 2.0f;

  static float x[8][3] = {{-d.x(), -d.y(), -d.z()}, {d.x(), -d.y(), -d.z()}, {d.x(), d.y(), -d.z()},
                          {-d.x(), d.y(), -d.z()},  {-d.x(), -d.y(), d.z()}, {d.x(), -d.y(), d.z()},
                          {d.x(), d.y(), d.z()},    {-d.x(), d.y(), d.z()}};

  static vtkIdType pts[6][4] = {{0, 1, 2, 3}, {4, 5, 6, 7}, {0, 1, 5, 4},
                                {1, 2, 6, 5}, {2, 3, 7, 6}, {3, 0, 4, 7}};

  vtkPolyData *cube = vtkPolyData::New();
  vtkPoints *points = vtkPoints::New();
  vtkCellArray *polys = vtkCellArray::New();
  vtkFloatArray *scalars = vtkFloatArray::New();

  size_t i;
  for (i = 0; i < 8; i++)
    points->InsertPoint(i, x[i]);
  for (i = 0; i < 6; i++)
    polys->InsertNextCell(4, pts[i]);

  scalars->InsertTuple1(0, 1);
  scalars->InsertTuple1(1, 1);
  scalars->InsertTuple1(2, 0);
  scalars->InsertTuple1(3, 0);
  scalars->InsertTuple1(4, 1);
  scalars->InsertTuple1(5, 1);
  scalars->InsertTuple1(6, 1);
  scalars->InsertTuple1(7, 1);

  cube->SetPoints(points);
  points->Delete();
  cube->SetPolys(polys);
  polys->Delete();
  cube->GetPointData()->SetScalars(scalars);
  scalars->Delete();

  return cube;
}

int runGrabberVisualisation(IIMUGrabber::Ptr grabber, float cubeSize,
                            const boost::filesystem::path &dataFile)
{
  auto logger = LoggingService::Instance().getLogger("runGrabber");

  grabber->open();
  if (grabber->isOpen())
  {
    logger->info("Grabber is open");
  }
  else
  {
    logger->error("Grabber failed to open");
    return 2;
  }

  vtkPolyData *cube = generateCube(Eigen::Vector3f(cubeSize, cubeSize * 0.1f, cubeSize));

  vtkPolyDataMapper *cubeMapper = vtkPolyDataMapper::New();
  cubeMapper->SetScalarRange(0, 1);
#if VTK_MAJOR_VERSION > 5
  cubeMapper->SetInputData(cube);
#else
  cubeMapper->SetInput(cube);
#endif

  vtkActor *cubeActor = vtkActor::New();
  cubeActor->SetMapper(cubeMapper);

  vtkRenderer *renderer = vtkRenderer::New();
  vtkRenderWindow *renderWindow = vtkRenderWindow::New();
  renderWindow->SetWindowName("IMU visualisation");
  renderWindow->AddRenderer(renderer);
  vtkRenderWindowInteractor *rendererInteractor = vtkRenderWindowInteractor::New();
  rendererInteractor->SetRenderWindow(renderWindow);

  rendererInteractor->Initialize();

  vtkSmartPointer<VTKIMUActorCallback> cb = vtkSmartPointer<VTKIMUActorCallback>::New();
  cb->setGrabber(grabber);
  cb->setActor(cubeActor);
  cb->setDataFile(dataFile);
  rendererInteractor->AddObserver(vtkCommand::TimerEvent, cb);
  rendererInteractor->CreateRepeatingTimer(10);

  vtkSmartPointer<VTKIMUCalibrationInteractionStyle> style =
      vtkSmartPointer<VTKIMUCalibrationInteractionStyle>::New();
  style->setGrabber(grabber);
  rendererInteractor->SetInteractorStyle(style);
  style->SetCurrentRenderer(renderer);

  renderer->AddActor(cubeActor);
  renderer->SetBackground(0.2, 0.4, 0.8);
  renderWindow->SetSize(300, 300);

  vtkAxes *axis = vtkAxes::New();
  vtkPolyDataMapper *axisMapper = vtkPolyDataMapper::New();
  axisMapper->SetInputConnection(axis->GetOutputPort());
  vtkActor *axisActor = vtkActor::New();
  axisActor->SetMapper(axisMapper);
  renderer->AddActor(axisActor);

  renderer->ResetCamera();
  renderWindow->Render();

  logger->info("Wait for device to wake...");
  std::this_thread::sleep_for(std::chrono::seconds(2));
  logger->info("ok.");

  rendererInteractor->Start();

  cube->Delete();
  cubeMapper->Delete();
  cubeActor->Delete();
  renderer->Delete();
  renderWindow->Delete();
  rendererInteractor->Delete();

  return 0;
}
