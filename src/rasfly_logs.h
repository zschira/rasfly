#ifndef _LOGS_H_
#define _LOGS_H_

#include <cstdio>
#include <string>

namespace rasfly {
	class logs {
	public:
		logs(std::string fname) {
			log_file = fopen(fname.c_str(), "w");
		}
		~logs() {
			fclose(log_file);
		}
		void write(std::string string) {
			fprintf(log_file, "%s\n", string.c_str());
		}
	private:
		FILE *log_file;
	};	

	logs logger("rasfly.log");
}

#endif