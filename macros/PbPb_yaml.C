 R__LOAD_LIBRARY(cpp_version/root_yaml.so)
#include "cpp_version/root_yaml.h"

void PbPb_yaml(int jetpt_bin = 7, int eta_bin = 4, bool dpt = false, int table = 0, bool doratio = false){

   gSystem->Load("cpp_version/root_yaml.so");

   bool fine = false;
   if(jetpt_bin < 10 && eta_bin == 4)fine = true;
   double eta_range[2][5];
   eta_range[0][0] = 0.0;
   eta_range[1][0] = 0.3;
   eta_range[0][1] = 0.3;
   eta_range[1][1] = 0.8;
   eta_range[0][2] = 0.8;
   eta_range[1][2] = 1.2;
   eta_range[0][3] = 1.2;
   eta_range[1][3] = 2.1;
   eta_range[0][4] = 0.0;
   eta_range[1][4] = 2.1;

   double jetpt_range[2][13];
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

   string centrality[6];
   centrality[0] = "0-10%";
   centrality[1] = "10-20%";
   centrality[2] = "20-30%";
   centrality[3] = "30-40%";
   centrality[4] = "40-60%";
   centrality[5] = "60-80%";
   // D(z) pp and 6 centraity bins, 5 jet pt bins, inclusive in y

   TFile *fppz, *fpbpbz;
  
   string var = "dpt";
   string rstring = "_PbPb";
   if(doratio) rstring = "_ratio";
   if(dpt == false) var = "ff";
   if(eta_bin == 4){
      if(!fine){
	 ostringstream fname;
	 fname << "../combined_sys/Uncertainties_eta_4_" << var << "_pp.root";
   	 fppz = new TFile(fname.str().c_str());
	 fname.clear();
	 fname.str("");
	 fname << "../combined_sys/Uncertainties_eta_4_" << var << rstring << ".root";
      	 fpbpbz = new TFile(fname.str().c_str());
      }else{
	 ostringstream fname;
	 fname << "../combined_sys/Uncertainties_eta_4_fine_" << var << "_pp.root";
	 fppz = new TFile(fname.str().c_str());
	 fname.clear();
	 fname.str("");
	 fname << "../combined_sys/Uncertainties_eta_4_fine_" << var << rstring << ".root";
	 fpbpbz = new TFile(fname.str().c_str());
      }
   }else{
      ostringstream ppname, pbpbname;
      ppname << "../combined_sys/Uncertainties_eta_" << eta_bin << "_" << var << "_pp" << ".root";
      fppz = new TFile(ppname.str().c_str());
      pbpbname << "../combined_sys/Uncertainties_eta_" << eta_bin << "_" << var << rstring << ".root";
      fpbpbz = new TFile(pbpbname.str().c_str());
      cout << ppname.str() << endl;
      cout << pbpbname.str() << endl;
   }

   root_yaml *ry = new root_yaml();
   ostringstream outs;
   if(table >= 0)outs << "pbpb_yaml/Table" << table << ".yaml";
   else outs << "pbpb_yaml/AuxTable" << abs(table) << ".yaml";
   ry->set_outfile(outs.str().c_str());
   ry->open_outfile();
//   ry->set_verbosity();


   std::vector<TH1 *> hstat_vec;
   std::vector<TH1 *> hsys_p_vec, hsys_n_vec; //positive and negative uncertainty variations
   std::vector<std::string> dep_var_qual;

   //include eta binning, fine binning and the values for the minimum x value for all the jet pt bins
   //options for dpt distributions, output this into separate files (at the bash script level?)
   double xmin = 0;
   double xmax = 1;
   if(!dpt){
      xmin = 1.0/jetpt_range[0][jetpt_bin];
      xmax = 1.05;
   }else{
      xmin = 1.0;
      xmax = jetpt_range[0][jetpt_bin];
   }


   ostringstream hname;
   if(!doratio){
   
      hname << "h_" << var << "_pp_" << jetpt_bin;
      TH1D *htmp = (TH1D *)fppz->Get(hname.str().c_str());
      hstat_vec.push_back(htmp);
      dep_var_qual.clear();

      hname.clear();
      hname.str("");
      hname << "h_TotalUncertainty_P_" << jetpt_bin;
      TH1D *htmp1 = (TH1D *)fppz->Get(hname.str().c_str());
      hsys_p_vec.push_back(htmp1);

      hname.clear();
      hname.str("");
      hname << "h_TotalUncertainty_N_" << jetpt_bin;
      TH1D *htmp2 = (TH1D *)fppz->Get(hname.str().c_str());
      hsys_n_vec.push_back(htmp2);

      ostringstream tmpss;
      tmpss.clear();
      tmpss << " - {name: RE, value: jet fragmentation function}" << endl;
      tmpss << " - {name: pT(jet), value: " << jetpt_range[0][jetpt_bin] << " - " << jetpt_range[1][jetpt_bin] << "}" << endl;
      tmpss << " - {name: abs. rapidity(jet), value: " << eta_range[0][eta_bin] << " - " << eta_range[1][eta_bin] << "}" << endl;
      tmpss << " - {name: SQRT(S), units: GeV, value: '5020.0'}" << endl;
      tmpss << " - {name: Centrality, value: pp}" << endl;
      dep_var_qual.push_back(tmpss.str());

      ry->add_dep_var_qualifiers(dep_var_qual);
      if(!dpt)ry->set_dep_variable_name("D(z)");
      if(dpt)ry->set_dep_variable_name("D(pT)");
      ry->set_dep_variable(htmp,htmp1, htmp2, xmin, xmax);
      ry->print_dep_variable();

   }
   ry->clear_dep_variables();
   TH1D *hi;
   for(int i=0; i<6; i++){ //centrality bins are from 0-5

      ostringstream hname;
      if(!doratio)hname << "h_" << var << "_PbPb_" << jetpt_bin << "_cent" << i;
      else hname << "h_" << var << "_ratio_" << jetpt_bin << "_cent" << i;
      TH1D *htmp = (TH1D *)fpbpbz->Get(hname.str().c_str());
      if(i==0)hi = (TH1D *)fpbpbz->Get(hname.str().c_str());
      dep_var_qual.clear();

      hname.clear();
      hname.str("");
      hname << "h_TotalUncertainty_P_" << jetpt_bin << "_cent" << i;
      TH1D *htmp1 = (TH1D *)fpbpbz->Get(hname.str().c_str());

      hname.clear();
      hname.str("");
      hname << "h_TotalUncertainty_N_" << jetpt_bin << "_cent" << i;
      TH1D *htmp2 = (TH1D *)fpbpbz->Get(hname.str().c_str());

      ostringstream tmpss;
      tmpss.clear();
      if(!doratio)tmpss << " - {name: RE, value: jet fragmentation function}" << endl;
      else tmpss << " - {name: RE, value: ratio of jet fragmentation function compared to pp collisions}" << endl;
      tmpss << " - {name: pT(jet), value: " << jetpt_range[0][jetpt_bin] << " - " << jetpt_range[1][jetpt_bin] << "}" << endl;
      tmpss << " - {name: abs. rapidity(jet), value: " << eta_range[0][eta_bin] << " - " << eta_range[1][eta_bin] << "}" << endl;
      tmpss << " - {name: SQRT(S), units: GeV, value: '5020.0'}" << endl;
      tmpss << " - {name: Centrality, value: " << centrality[i] << "}" << endl;
      dep_var_qual.push_back(tmpss.str());

      ry->add_dep_var_qualifiers(dep_var_qual);
      if(!doratio && !dpt)ry->set_dep_variable_name("D(z)");
      if(!doratio && dpt)ry->set_dep_variable_name("D(pT)");
      if(doratio && !dpt)ry->set_dep_variable_name("R_D(z)");
      if(doratio && dpt)ry->set_dep_variable_name("R_D(pT)");
   
      ry->set_dep_variable(htmp,htmp1, htmp2, xmin, xmax);
      ry->print_dep_variable();
      ry->clear_dep_variables();
   }
      
   ry->set_ind_variable(hi,xmin, xmax);
   if(!dpt)ry->set_ind_variable_name("z");
   if(dpt)ry->set_ind_variable_name("pT","GEV");
   ry->print_ind_variable();

}
