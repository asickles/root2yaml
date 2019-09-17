#include "root_yaml.h"
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

ClassImp(root_yaml);
root_yaml::root_yaml(){

   cout << "hello work!" << endl;
   cout << bin << endl;
   blah.push_back(4);
   string tmps = "  qualifiers:";
   ind_var_qualifiers.push_back(tmps);
   verbosity = false;
   outfile = "test.yaml";
}

root_yaml::~root_yaml(){

}

void root_yaml::set_verbosity(bool val){
   verbosity = val;
}

void root_yaml::set_outfile(string outf){
   outfile = outf;
}

void root_yaml::open_outfile(){
   ofs.open(outfile.c_str());
   cout << "open? " << ofs.is_open() << endl;
	
}

void root_yaml::print_header(){
   cout << "hello world" << endl;
}

void root_yaml::add_ind_var_qualifier(std::string name, std::string value){
   ostringstream tmp;
   tmp << "  - {name: " << name << ", value: " << value << "}";
   ind_var_qualifiers.push_back(tmp.str());
}


void root_yaml::add_dep_var_qualifiers(std::vector<std::string> qual){
   dep_var_qualifiers = qual;
}

void root_yaml::set_ind_variable(TH1 *htmp, double xmin, double xmax){
   for(int i=0; i<=htmp->GetNbinsX(); i++){
      double x = htmp->GetBinLowEdge(i);
      if(verbosity) cout << "set ind var: " << x << " " << xmin << " " << xmax << endl;
      if(verbosity) cout << "set ind var: " << i << " " << htmp->GetNbinsX() << endl;
      if(x >= xmin && x <= xmax) ind_var.push_back(x);
   }
}

void root_yaml::set_ind_variable(TGraphErrors *gtmp){

   int npts = gtmp->GetN();
   for(int i=0; i<npts; i++){
      double x,y;
      gtmp->GetPoint(i,x,y);
      ind_var.push_back(x);
   }
}

void root_yaml::set_ind_variable_name(std::string name, std::string units){
   ind_var_name = name;
   ind_var_units = units;
}

void root_yaml::set_dep_variable_name(std::string name){
   dep_var_name = name;
}

void root_yaml::set_dep_variable(TH1 *hstat, TH1 *hsys_p, TH1 *hsys_n, double xmin, double xmax){
   for(int i=0; i<hstat->GetNbinsX(); i++){
      double val = hstat->GetBinContent(i+1);
      double err = hstat->GetBinError(i+1);
      double x = hstat->GetBinCenter(i+1);
      double xlow = hstat->GetBinLowEdge(i+1);
      double xhigh = xlow + hstat->GetBinWidth(i+1);
      double sys_p = (hsys_p->GetBinContent(i+1) - 1.0) * val;
      double sys_n = (hsys_n->GetBinContent(i+1) - 1.0) * val;
      if(verbosity)cout << x << " " << val << " " << err << "dep variables" << endl;
      if(xlow >= xmin && xhigh <= xmax){
	 if(verbosity) cout << "accept" << endl;
	 dep_var.push_back(val);
	 dep_var_stat.push_back(err);
	 dep_var_syslow.push_back(sys_n);
	 dep_var_syshigh.push_back(sys_p);
      }
   }
}

void root_yaml::set_dep_variable(TGraphErrors *gstat, TGraphAsymmErrors *gsys){

   int npts = gstat->GetN();
   for(int i=0; i<npts; i++){
      double x, y, ystat, ysys_l, ysys_h;
      gstat->GetPoint(i,x,y);
      ystat = gstat->GetErrorY(i);
      ysys_l = gsys->GetErrorYlow(i);
      ysys_h = gsys->GetErrorYhigh(i);
      dep_var.push_back(y);
      dep_var_stat.push_back(ystat);
      dep_var_syslow.push_back(ysys_l);
      dep_var_syshigh.push_back(ysys_h);
   }

}

void root_yaml::set_dep_variable(vector<TH1 *> hstat_v, vector<TH1 *> hsys_p_v, vector<TH1 *> hsys_n_v, vector<double> xmin, vector<double> xmax){
   if(verbosity)cout << "number of histograms: " << hstat_v.size() << endl;
   for(int ih = 0; ih < hstat_v.size(); ih++){

      /*
      if(verbosity)cout << hstat_v[ih]->GetNbinsX() << endl;
      vector<double> tmp_vec, tmp_stat, tmp_p_sys, tmp_n_sys;
      for(int i=0; i<hstat_v[ih]->GetNbinsX(); i++){
	 double val = hstat_v[ih]->GetBinContent(i+1);
	 double err = hstat_v[ih]->GetBinError(i+1);
	 double x = hstat_v[ih]->GetBinCenter(i+1);
	 double sys_p = (hsys_p_v[ih]->GetBinContent(i+1) - 1.0) * val;
	 double sys_n = (hsys_n_v[ih]->GetBinContent(i+1) - 1.0) * val;
	 if(verbosity)cout << "in here: " << xmin[ih] << " " << xmax[ih] << " " << val << endl;
	 if(x > xmin[ih] && x < xmax[ih]){
	    if(verbosity) cout << x << " "  << val << endl;
	    tmp_vec.push_back(val);
	    tmp_stat.push_back(err);
	    tmp_p_sys.push_back(sys_p);
	    tmp_n_sys.push_back(sys_n);
	 }
      }
      dep_var.push_back(tmp_vec);
      dep_var_stat.push_back(tmp_stat);
      dep_var_syslow.push_back(tmp_n_sys);
      dep_var_syshigh.push_back(tmp_p_sys);
      */
   }

   if(verbosity)cout << "end of set dep variable" << endl;

}

//prints...

void root_yaml::print_dep_var_qualifiers(){
   ofs << "  qualifiers:" << endl;
   for (vector<string>::iterator it = dep_var_qualifiers.begin() ; it != dep_var_qualifiers.end(); ++it)
    ofs << *it ;
}
void root_yaml::print_ind_variable(){

   ofs << "independent_variables:" << endl;
   if(ind_var_units.size()==0)ofs << "- header: {name: " << ind_var_name << "}" << endl;
   else ofs << "- header: {name: " << ind_var_name << ", units: " << ind_var_units << "}" << endl;
   ofs << "  values:" << endl;
   if(verbosity)cout << "ind var size: " << ind_var.size() << " " << ind_var[ind_var.size()-1] << endl;
   for(int i=0; i<ind_var.size()-1; i++){
      ofs << "  - {high: " << ind_var[i+1] << ", low: " << ind_var[i] << "}" << endl;
   }

}
void root_yaml::print_dep_variable(){

	if (first_dep_var)
	{
		ofs << "dependent_variables:" << endl;
		first_dep_var = false;
	}

   ofs << "- header: {name: " << dep_var_name << "}" << endl;
   print_dep_var_qualifiers();
      
   ofs << "  values: " << endl;
   
   for(int i = 0; i < dep_var.size(); i++){
	 
      ofs << "  - errors:" << endl;
      ofs << "    - {label: stat, symerror: " << dep_var_stat[i] << "}" << endl;
      ofs << "    - asymerror: {minus: " << dep_var_syslow[i] << ", plus: " << dep_var_syshigh[i] << "}" << endl;
      ofs << "      label: sys" << endl;
      ofs << "    value: " << dep_var[i] << endl;

   }
}

void root_yaml::clear_dep_variables(){

   dep_var.clear();
   dep_var_syslow.clear();
   dep_var_syshigh.clear();
   dep_var_stat.clear();
   dep_var_qualifiers.clear();

   dep_var.resize(0);
   dep_var_syslow.resize(0);
   dep_var_syshigh.resize(0);
   dep_var_stat.resize(0);
   dep_var_qualifiers.resize(0);


}
