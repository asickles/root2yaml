 R__LOAD_LIBRARY(cpp_version/root_yaml.so)
#include "cpp_version/root_yaml.h"

void PbPb_sums(int doptweight = false){

   gSystem->Load("cpp_version/root_yaml.so");


   double jetpt_range[2][15];
   jetpt_range[0][0] = 25.119;
   jetpt_range[1][0] = 31.623;
   jetpt_range[0][1] = 31.623;
   jetpt_range[1][1] = 40.0;
   jetpt_range[0][2] = 40.0;
   jetpt_range[1][2] = 50.119;
   jetpt_range[0][3] = 50.119;
   jetpt_range[1][3] = 63;
   jetpt_range[0][4] = 63;
   jetpt_range[1][4] = 80;
   jetpt_range[0][5] = 80;
   jetpt_range[1][5] = 100;
   jetpt_range[0][6] = 100;
   jetpt_range[1][6] = 126.0;
   jetpt_range[0][7] = 126.0;
   jetpt_range[1][7] = 158.488;
   jetpt_range[0][8] = 158.488;
   jetpt_range[1][8] = 199.525;
   jetpt_range[0][9] = 199.525;
   jetpt_range[1][9] = 251.186;
   jetpt_range[0][10] = 251.186;
   jetpt_range[1][10] = 316.224;
   jetpt_range[0][11] = 316.224;
   jetpt_range[1][11] = 398.101;
   jetpt_range[0][12] = 398.101;
   jetpt_range[1][12] = 501.178;
   jetpt_range[0][13] = 501.178;
   jetpt_range[1][13] = 501.178;
   jetpt_range[0][14] = 521.178;
   jetpt_range[1][14] = 541.178;

   string centrality[6];
   centrality[0] = "0-10%";
   centrality[1] = "10-20%";
   centrality[2] = "20-30%";
   centrality[3] = "30-40%";
   centrality[4] = "40-60%";
   centrality[5] = "60-80%";
   // D(z) pp and 6 centraity bins, 5 jet pt bins, inclusive in y

   double jetarray[8];
   for(int i=0; i<8; i++){
      jetarray[i] = jetpt_range[0][i+7];
   }
   for(int i=0; i<7; i++){
      cout << jetarray[i] << endl;
   }
   TH1 *hjetptranges = new TH1D("hjetptranges","hjetptranges",7,jetarray);

   root_yaml *ry = new root_yaml();
//   ry->set_verbosity();

   ostringstream outs;
   if(doptweight) outs << "pbpb_yaml/Table" << 201 << ".yaml";
   else outs << "pbpb_yaml/Table" << 202 << ".yaml";
   ry->set_outfile(outs.str().c_str());
   ry->open_outfile();
   for(int i=0; i<6; i++){
      ostringstream inf;
      inf << "/Users/sickles/Dropbox/systematic_variations/dpt_ratio_cent" << i << "_fine_eta4.root";

      TFile *fin = new TFile(inf.str().c_str());

      string name_stat, name_sys;
      if(!doptweight){
	 name_stat = "gparticles";
	 name_sys = "gparticles_sys";
      }else{
	 name_stat = "gpt";
	 name_sys = "gpt_sys";
      }

      TGraphErrors *gstat = (TGraphErrors *)fin->Get(name_stat.c_str());
      TGraphAsymmErrors *gsys = (TGraphAsymmErrors *)fin->Get(name_sys.c_str());

      std::vector<std::string> dep_var_qual;

      ostringstream tmpss;
      tmpss.clear();
      if(!doptweight)
	 tmpss << "  - {name: RE, value: excess particles in jet in PbPb compared to pp collisions}" << endl;
      else
	 tmpss << "  - {name: RE, value: excess transverse momenta in jet in PbPb compared to pp collisions}" << endl;
      tmpss << "  - {name: abs. rapidity(jet), value: " << 0.0 << " - " << 2.1 << "}" << endl;
      tmpss << "  - {name: SQRT(S), units: GeV, value: '5020.0'}" << endl;
      tmpss << "  - {name: centrality, value: '" << centrality[i] << "'}" << endl;
      dep_var_qual.push_back(tmpss.str());

      ry->add_dep_var_qualifiers(dep_var_qual);
      if(!doptweight)ry->set_dep_variable_name("delta N");
      if(doptweight)ry->set_dep_variable_name("delta pt");
      ry->set_dep_variable(gstat,gsys);
      ry->print_dep_variable();
   
      ry->clear_dep_variables();
      
   
      if(i==5)ry->set_ind_variable(hjetptranges,125,502);
      ry->set_ind_variable_name("pT_jet","GEV");
      if(i==5)ry->print_ind_variable();
   }

}
