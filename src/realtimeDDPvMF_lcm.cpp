/* Copyright (c) 2015, Julian Straub <jstraub@csail.mit.edu> Licensed
 * under the MIT license. See the license file LICENSE.
 */

#include <iostream>
#include <fstream>
#include <string>

#include <rtDDPvMF/rtDDPvMF.hpp>
#include <rtDDPvMF/realtimeDDPvMF_lcm.hpp>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

int main (int argc, char** argv)
{

  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
//    ("K,K", po::value<int>(), "K for spkm clustering")
    ("lambdaDeg,l", po::value<double>(), "lambda in degree for dp and ddp")
    ("beta,b", po::value<double>(), "beta parameter of the ddp")
    ("nFramesSurvive,s", po::value<int>(), "number of frames a cluster survives without observation")
//    ("mode", po::value<string>(), "mode of the rtDDPvMF (spkm, dp, ddp)")
    ("in,i", po::value<string>(), "path to input file")
    ("out,o", po::value<string>(), "path to output file")
    ("display,d", "display results")
    ("B,B", po::value<int>(), "B for guided filter")
    ("eps", po::value<float>(), "eps for guided filter")
    ("f_d,f", po::value<float>(), "focal length of depth camera")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    cout << desc << "\n";
    return 1;
  }

  CfgRtDDPvMF cfg;
  cfg.f_d = 540.;
  cfg.beta = 1e5;
  cfg.nFramesSurvive_ = 300;
//  cfg.nSkipFramesSave = 60;
  cfg.pathOut = std::string("../results/");
  double lambdaDeg = 93.;
  int K = -1;
//  if(vm.count("K")) K = vm["K"].as<int>();
  if(vm.count("lambdaDeg")) lambdaDeg = vm["lambdaDeg"].as<double>();
  if(vm.count("nFramesSurvive")) cfg.nFramesSurvive_ = vm["nFramesSurvive"].as<int>();
  if(vm.count("beta")) cfg.beta = vm["beta"].as<double>();

  cfg.lambdaFromDeg(lambdaDeg);
  cfg.QfromFrames2Survive(cfg.nFramesSurvive_);

  if(cfg.nFramesSurvive_==0)
    cfg.pathOut += "dp_fbf/";
  else if(cfg.nFramesSurvive_==1)
    cfg.pathOut += "dp/";
  else if(K>0)
    cfg.pathOut += "spkm/";
  else
    cfg.pathOut += "ddp/";

  string path = "";
//  string mode = "";
  cudaPcl::CfgSmoothNormals cfgNormals;
  cfgNormals.f_d = 540.;
  cfgNormals.eps = 0.2*0.2;
  cfgNormals.B = 9;
  cfgNormals.compress = true;
  uint32_t T = 10;
//  if(vm.count("mode")) mode = vm["mode"].as<string>();
  if(vm.count("in")) path = vm["in"].as<string>();
  if(vm.count("eps")) cfgNormals.eps = vm["eps"].as<float>();
  if(vm.count("f_d")) cfgNormals.f_d = vm["f_d"].as<float>();
  if(vm.count("B")) cfgNormals.B = uint32_t( vm["B"].as<int>());

  findCudaDevice(argc,(const char**)argv);
//  if(K<0)
//  {
    cout<<"rtDDPvMFmeans lambdaDeg="<<cfg.lambdaDeg_<<" beta="<<cfg.beta
      <<"nFramesSurvive="<<cfg.nFramesSurvive_<<endl;
    cout<<"output path: "<<cfg.pathOut<<endl;
    shared_ptr<RtDDPvMF> pRtDdpvMF(new RtDDPvMF(cfg,cfgNormals));

    shared_ptr<lcm::LCM> lcm(new lcm::LCM);
    if(!lcm->good()){
      cerr <<"ERROR: lcm is not good()" <<endl;
    }

    RealtimeDDPvMF_lcm app(lcm,pRtDdpvMF,channel,outPath);
    while(0 == lcm->handle());
//  }else{
//    cout<<"rtSpkm K="<<K<<endl;
//    cout<<"output path: "<<cfg.pathOut<<endl;
//    shared_ptr<RtSpkm> pRtSpkm(new RtSpkm(cfg.pathOut,K,cfgNormals));
//    RealtimeSpkm v(pRtSpkm);
//    v.run ();
//  }
  cout<<cudaDeviceReset()<<endl;
  return (0);
}
