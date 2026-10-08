{
   const Int_t NRGBs = 5;
   const Int_t NCont = 255;

   Double_t stops[NRGBs] = { 1.00, 0.34, 0.61, 0.84, 1.00 };
   Double_t red[NRGBs]   = { 1.00, 0.00, 0.87, 1.00, 0.51 };
   Double_t green[NRGBs] = { 1.00, 0.81, 1.00, 0.20, 0.00 };
   Double_t blue[NRGBs]  = { 1., 1.00, 0.12, 0.00, 0.00 };
   TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
   gStyle->SetNumberContours(NCont);
   TString dypath = gSystem->GetDynamicPath();
   TString incpath = gSystem->GetIncludePath();

   // style
   gStyle->SetOptStat(1111111);
      

   
   // local
   dypath.Append(":./src");
   dypath.Append(":./build/src");
   incpath.Append(gSystem->GetFromPipe("artemis-config --cflags"));
   incpath.Append(" -Isrc");

   // share
   // dypath.Append(":${ART_SHARE_DIR}/lib");
   gSystem->SetDynamicPath(dypath);
   gSystem->SetIncludePath(incpath);

   // load libraries
   gSystem->Load("libMinuit");
   gSystem->Load("libart_test");
   
   

   TCatCmdFactory *cf = TCatCmdFactory::Instance();
   cf->SetOptExactName(kFALSE);
   cf->Register(TCatCmdHelp::Instance());
   cf->Register(TCatCmdHt::Instance());
   cf->Register(TCatCmdHtp::Instance());
   cf->Register(TCatCmdHb::Instance());
   cf->Register(TCatCmdHn::Instance());
   cf->Register(TCatCmdZone::Instance());
   cf->Register(TCatCmdLs::Instance());
   cf->Register(TCatCmdCd::Instance());
   cf->Register(TCatCmdPrx::Instance());
   cf->Register(TCatCmdPry::Instance());
   cf->Register(TCatCmdAvx::Instance());
   cf->Register(TCatCmdAvy::Instance());
   cf->Register(TCatCmdBnx::Instance());
   cf->Register(TCatCmdBny::Instance());
   cf->Register(new TCatCmdLg(TCatCmdLg::kX,0));
   cf->Register(new TCatCmdLg(TCatCmdLg::kX,1));
   cf->Register(new TCatCmdLg(TCatCmdLg::kY,0));
   cf->Register(new TCatCmdLg(TCatCmdLg::kY,1));
   cf->Register(new TCatCmdLg(TCatCmdLg::kZ,0));
   cf->Register(new TCatCmdLg(TCatCmdLg::kZ,1));
   cf->Register(TCatCmdSly::Instance());
   gInterpreter->ProcessLine("TCatCmdFactory::Instance()->Register(art::TCmdLoopAdd::Instance(), NULL, NULL, kTRUE);");
   gInterpreter->ProcessLine("TCatCmdFactory::Instance()->Register(art::TCmdCheckYAML::Instance(), NULL, NULL, kTRUE);");
   gInterpreter->ProcessLine("TCatCmdFactory::Instance()->Register(art::TCmdShowKey::Instance(), NULL, NULL, kTRUE);");
   gInterpreter->ProcessLine("TCatCmdFactory::Instance()->Register(art::TCmdShowYAML::Instance(), NULL, NULL, kTRUE);");
   cf->Register(TCatCmdLoopResume::Instance());
   cf->Register(TCatCmdLoopSuspend::Instance());
   cf->Register(TCatCmdLoopTerminate::Instance());
   cf->Register(new TCatCmdHstore);
   cf->Register(TCatCmdXval::Instance());
   cf->Register(art::TCatCmdListg::Instance());

   cf->Register(new art::TCmdBranchInfo);
   cf->Register(new art::TCmdClassInfo);
   cf->Register(new art::TCmdHdel);
   cf->Register(new art::TCmdFileCd);
   cf->Register(new art::TCmdFileLs);
   cf->Register(art::TCmdPn::Instance());
   cf->Register(art::TCmdPb::Instance());
   cf->Register(art::TCmdPcd::Instance());
   cf->Register(new art::TCmdRg(art::TCmdRg::kX));
   cf->Register(new art::TCmdRg(art::TCmdRg::kY));
   cf->Register(new art::TCmdRg(art::TCmdRg::kZ));
   cf->Register(new art::TCmdSlope);
   cf->Register(art::TCmdPn::Instance());
   cf->Register(art::TCmdPb::Instance());
   cf->Register(art::TCmdPcd::Instance());
   cf->Register(art::TCmdPadZoom::Instance());
   cf->Register(new art::TCmdProcessorDescription);
   cf->Register(new art::TCmdUnZoom);
   cf->Register(new art::TCmdComment);
   cf->Register(new art::TCmdGlobalComment);

   art::TCmdSave * cmdsave = art::TCmdSave::Instance();
   cmdsave->SetDefaultDirectory("figs");
   cmdsave->SetAddDateDir(kTRUE);
   cmdsave->SetAutoName(kTRUE);
   cmdsave->AddFormat("png");
   cmdsave->AddFormat("root");
   cmdsave->AddFormat("pdf",1);
   cf->Register(cmdsave);
   art::TCmdPrint *pri = new art::TCmdPrint;
   pri->SetCommand("pdf2ps %s tmp.ps && lp -o fit-to-page tmp.ps");
//   pri->SetOption("-o fit-to-page -o number-up=2");
   cf->Register(pri);

   {

   }
   {
      art::TModuleDecoderFactory *df = art::TModuleDecoderFactory::Instance();
// mod ID 0 : Fixed16
      const Int_t digits0 = 16;
      // skip scaler data in RIBF79 tentatively
      df->Register(new art::TModuleDecoderSkip(0));
      //      df->Register(new art::TModuleDecoderFixed<unsigned short>(0,digits0) );
// mod ID 1 : Fixed24
      const Int_t digits1 = 24;
      df->Register(new art::TModuleDecoderFixed<unsigned int>(1,digits1) );
// mod ID 21 : V7XX
      df->Register(new art::TModuleDecoderV7XX);
// mod ID 23 : V767
      df->Register(new art::TModuleDecoderV767);
// mod ID 24 : V1190
// mod ID 25 : V1290
// mod ID 26 : V1190C
      art::TModuleDecoder *v1190 = new art::TModuleDecoderV1190;
      art::TModuleDecoder *v1290 = new art::TModuleDecoderV1290(25);
      art::TModuleDecoder *v1190c = new art::TModuleDecoderV1190(26);
      v1190->SetVerboseLevel(kError);
      v1290->SetVerboseLevel(kError);
      v1190c->SetVerboseLevel(kError);
      df->Register(v1190);
      df->Register(v1290);
      df->Register(v1190c);

// mod ID 39 : MDPPQDC                                                                                                                                                                                      
      gInterpreter->ProcessLine("art::TModuleDecoderFactory::Instance()->Register(new art::TModuleDecoderMDPPQDC);");
// mod ID 63 : V1740
      //df->Register(new art::TModuleDecoderV1740);
//      df->Register(new art::TModuleDecoderV1740_mod);
// mod ID 31 : A3100
//      df->Register(new art::TModuleDecoderA3100);
//      df->Register(new art::TModuleDecoderHINP);
// mod ID  8 : SIS3301
//      df->Register(new art::TModuleDecoderSIS3301);

// mod ID 36 : SIS3820
      df->Register(new art::TModuleDecoderSIS3820);
      df->Register(new art::TModuleDecoderSIS3610);
      
      df->Register(new art::TModuleDecoderSkip(8));     
      df->Register(new art::TModuleDecoderSkip(42)); 

      
//      TArtDecoderFactory *df = TArtDecoderFactory::Instance();
//      df->Register(new art::TModuleDecoderSIS3610);
//      df->Register(new art::TModuleDecoderSIS3820);



//      df->Register(new art::TModuleDecoderSkip(25));
//      TArtDecoderFactory *df = TArtDecoderFactory::Instance();
//      df->Register(TArtDecoderSIS3610::Instance());
//      df->Register(TArtDecoderSIS3820::Instance());

////      TArtDecoderV1190 *dc = (TArtDecoderV1190*)TArtDecoderV1190::Instance();
////      dc->SetTrailingChOffset(128);
////      df->Register(TArtDecoderV1190::Instance());
//      df->Register(new TCatDecoderV1190);
//      df->Register(TArtDecoderFixed16::Instance());
//      df->Register(TArtDecoderV7XX::Instance());
////      df->Register(TCatDecoderV1740::Instance());
   }
}
