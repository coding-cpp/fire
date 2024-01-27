#ifdef __linux__
#  pragma once

#  include <chrono>
#  include <cstring>
#  include <dirent.h>
#  include <errno.h>
#  include <fcntl.h>
#  include <iostream>
#  include <limits.h>
#  include <queue>
#  include <sys/inotify.h>
#  include <unistd.h>
#  include <unordered_map>
#  include <vector>

class InotifyWatcher {
public:
  InotifyWatcher(const std::string& path)
      : rootPath(path) {
    fd = inotify_init1(IN_NONBLOCK);
    if (fd < 0) {
      std::cerr << "inotify_init1 failed" << std::endl;
      exit(EXIT_FAILURE);
    }
    addWatchRecursively(path);
  }

  ~InotifyWatcher() {
    for (const auto& pair : watchDescriptors) {
      inotify_rm_watch(fd, pair.first);
    }
    close(fd);
  }

  void addWatch(const std::string& path) {
    int wd = inotify_add_watch(fd, path.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVE);
    if (wd == -1) {
      std::cerr << "Cannot watch '" << path << "': " << strerror(errno) << std::endl;
    } else {
      std::cout << "Watching '" << path << "' using wd " << wd << std::endl;
      watchDescriptors[wd] = path;
    }
  }

  void addWatchRecursively(const std::string& path) {
    std::queue<std::string> directories;
    directories.push(path);

    while (!directories.empty()) {
      std::string currentPath = directories.front();
      directories.pop();

      addWatch(currentPath);

      DIR* dir = opendir(currentPath.c_str());
      if (dir == nullptr) {
        std::cerr << "Failed to open directory: " << currentPath << std::endl;
        continue;
      }

      struct dirent* entry;
      while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_DIR) {
          std::string name = entry->d_name;
          if (name != "." && name != "..") {
            directories.push(currentPath + "/" + name);
          }
        }
      }

      closedir(dir);
    }
  }

  void removeWatch(int wd) {
    if (inotify_rm_watch(fd, wd) == -1) {
      std::cerr << "Error removing watch: " << strerror(errno) << std::endl;
    } else {
      watchDescriptors.erase(wd);
    }
  }

  void run() {
    while (true) {
      readEvents();
    }
  }

private:
  int fd;
  std::string rootPath;
  std::unordered_map<int, std::string> watchDescriptors;
  std::unordered_map<std::string, std::chrono::steady_clock::time_point> lastModified;

  void readEvents() {
    char buffer[4096] __attribute__((aligned(__alignof__(struct inotify_event))));
    const struct inotify_event* event;
    ssize_t length;

    while ((length = read(fd, buffer, sizeof buffer)) > 0) {
      for (char* ptr = buffer; ptr < buffer + length; ptr += sizeof(struct inotify_event) + event->len) {
        event = (const struct inotify_event*) ptr;

        // Process the event
        handleEvent(event);
      }
    }

    if (length < 0 && errno != EAGAIN) {
      std::cerr << "read error: " << strerror(errno) << std::endl;
    }
  }

  void handleEvent(const struct inotify_event* event) {
    auto now = std::chrono::steady_clock::now();
    if (event->len) {
      std::string path = watchDescriptors[event->wd] + '/' + event->name;

      if (event->mask & IN_CREATE) {
        std::cout << "Created: " << path << std::endl;
        if (event->mask & IN_ISDIR) {
          addWatchRecursively(path);
        }
      } else if (event->mask & IN_DELETE) {
        std::cout << "Deleted: " << path << std::endl;
        if (event->mask & IN_ISDIR) {
          // Remove the watch if a directory is deleted
          removeWatch(event->wd);
        }
      } else if (event->mask & IN_MODIFY) {
        if (lastModified.find(path) != lastModified.end() && std::chrono::duration_cast<std::chrono::milliseconds>(now - lastModified[path]).count() < 100) {
          // If the file was modified less than a second ago, ignore this event
          return;
        }
        lastModified[path] = now;
        std::cout << "Modified: " << path << std::endl;
      } else if (event->mask & IN_MOVED_FROM || event->mask & IN_MOVED_TO) {
        std::cout << "Moved: " << path << std::endl;
        if (event->mask & IN_ISDIR) {
          removeWatch(event->wd);
        }
      }
    }
  }
};

#endif