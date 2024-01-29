#include <fire/platform/linux/pm.hpp>

namespace fire {
ProcessManager::ProcessManager() {
  this->buildLogger = std::make_shared<Logger>(Logger::Application::BUILD);
  this->runnerLogger = std::make_shared<Logger>(Logger::Application::RUNNER);
  this->cmd = Parser::get<std::string>("cmd", "", "build");
  if (cmd == "") {
    buildLogger->log("No cmd specified, exiting");
    exit(EXIT_FAILURE);
  }
  this->getBuildArgs();
  auto args_raw = *Parser::get_array<std::string>("args_bin", "build");
  for (const auto& arg : args_raw) {
    this->args.push_back(const_cast<char*>(arg.c_str()));
  }
  // Remove its correct here
  std::cout << this->buildArgs[0] << std::endl;
  std::cout << this->buildArgs[1] << std::endl;
  std::cout << this->buildArgs[2] << std::endl;
  // output
  // -j10
  // -C
  // build
}

ProcessManager::~ProcessManager() {
  if (this->pid > 0) {
    kill(this->pid, SIGTERM);
    waitpid(this->pid, NULL, 0);  // Wait for process to terminate
  }
}

void ProcessManager::stopProcess() {
  if (this->pid > 0) {
    kill(this->pid, SIGTERM);
    waitpid(this->pid, NULL, 0);  // Wait for process to terminate
    this->pid = -1;
  }
}

void ProcessManager::startProcess() {
  if (pipe(this->pipefd) == -1) {
    buildLogger->log("Failed to create pipe");
    exit(EXIT_FAILURE);
  }
  this->pid = fork();
  if (this->pid == 0) {
    // Child process
    close(this->pipefd[0]);  // Close unused read end
    dup2(this->pipefd[1], STDOUT_FILENO);
    close(this->pipefd[1]);  // Close write end of pipe
    buildLogger->log("Building: " + this->cmd);
    // Remove the fault
    std::cout << this->buildArgs[0] << std::endl;
    std::cout << this->buildArgs[1] << std::endl;
    std::cout << this->buildArgs[2] << std::endl;
    // output
    // lude/fire/platform/windows
    // -C
    // build
    execvp(this->cmd.c_str(), this->buildArgs.data());
    // Starting binary with args
    runnerLogger->log("Running binary: " + Parser::get<std::string>("bin", "", "build"));
    execvp(Parser::get<std::string>("bin", "", "build").c_str(), this->args.data());
    exit(EXIT_FAILURE);

  } else if (this->pid > 0) {
    // Parent process
    close(pipefd[1]);  // Close unused write end
    char buffer[128];
    ssize_t count;
    while ((count = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
      write(STDOUT_FILENO, buffer, count);
    }
    close(pipefd[0]);                // Close read end
    waitpid(this->pid, nullptr, 0);  // Wait for child process to end
  } else {
    // Fork failed
    std::cerr << "Failed to fork process" << std::endl;
    close(pipefd[0]);
    close(pipefd[1]);
  }
}

void ProcessManager::restartProcess() {
  this->stopProcess();
  this->startProcess();
}

void ProcessManager::getBuildArgs() {
  std::istringstream iss(this->cmd);
  std::vector<std::string> results;
  std::string word;
  while (iss >> word) {
    results.push_back(word);
  }
  this->cmd = results[0];
  for (int i = 1; i < results.size(); i++) {
    this->buildArgs.push_back(const_cast<char*>(results[i].c_str()));
  }
}

}  // namespace fire