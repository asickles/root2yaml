#ifndef __ROOT_YAML_H__
#define __ROOT_YAML_H__

#include "Rtypes.h"
#include <string>
#include <vector>
#include "TH1.h"
#include <fstream>

class root_yaml {
   private:
      int bin;
      bool verbosity;
      std::vector<int> blah;
      std::vector<std::string> ind_var_qualifiers;
      std::vector<std::string> dep_var_qualifiers;
      std::vector<double> ind_var;
      std::vector<double> dep_var;
      std::vector<double> dep_var_stat;
      std::vector<double> dep_var_syslow;
      std::vector<double> dep_var_syshigh;
      std::string ind_var_name;
      std::string dep_var_name;
      std::string ind_var_units;
      std::string outfile;
      std::ofstream ofs;
      bool first_dep_var = true;
   public:
      root_yaml();
      ~root_yaml();
      void set_outfile(std::string outf);
      void open_outfile();
      void set_verbosity(bool val = true);
      void print_header();
      void add_ind_var_qualifier(std::string name, std::string value);
      void add_dep_var_qualifiers(std::vector<std::string> qual);
      void print_dep_var_qualifiers();
      void print_ind_var_qualifiers();
      void set_ind_var_qualifiers();
      void set_dep_var_qualifiers();
      void set_ind_variable(TH1 *htmp, double xmin, double xmax);
      void set_ind_variable_name(std::string name, std::string units = "");
      void set_dep_variable_name(std::string name);
      void set_dep_variable(TH1 *hstat, TH1 *hsys_p, TH1 *hsys_n, double xmin = 0, double xmax = 1.0);
      void set_dep_variable(std::vector<TH1 *> hstat_v, std::vector<TH1 *> hsys_p_v, 
	    std::vector<TH1 *> hsys_n_v, std::vector<double> xmin, std::vector<double> xmax);
      void print_ind_variable();
      void print_dep_variable();
      void clear_dep_variables();
      ClassDef(root_yaml,1);
};
#endif
