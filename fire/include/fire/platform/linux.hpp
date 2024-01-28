#ifdef __linux__
#  pragma once

#  include <fire/parser.hpp>
#  include <fire/watcher.hpp>
#  include <iostream>
#  include <sys/inotify.h>
#  include <unistd.h>
#  include <vector>

namespace fire {

class InotifyWatcher : private Watcher {
private:
  int fd;
  std::string rootPath;
  std::unordered_map<int, std::string> watchDescriptors;
  std::unordered_map<std::string, std::chrono::steady_clock::time_point> lastModified;

  void addWatch(const std::string& path);
  // void addWatchRecursively(const std::string& path);
  void removeWatch(int wd);
  void startWatch();
  void handleEvent(const struct inotify_event* event);

public:
  InotifyWatcher(const std::string& path);
  ~InotifyWatcher();
  void watch();
};

}  // namespace fire

#endif