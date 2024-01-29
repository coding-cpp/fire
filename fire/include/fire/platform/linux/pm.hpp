#ifdef __linux__
#  pragma once

#  include <fire/logger.hpp>
#  include <fire/parser.hpp>
#  include <iostream>
#  include <signal.h>
#  include <sys/types.h>
#  include <sys/wait.h>
#  include <unistd.h>

namespace fire {

class ProcessManager {
private:
  pid_t pid = -1;
  int pipefd[2];
  std::string cmd;
  std::vector<char*> buildArgs;
  std::vector<char*> args;
  std::shared_ptr<Logger> buildLogger;
  std::shared_ptr<Logger> runnerLogger;

public:
  ProcessManager();
  ~ProcessManager();

  void startProcess();
  void stopProcess();
  void restartProcess();
  void getBuildArgs();
};

}  // namespace fire

#endif