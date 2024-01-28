#ifdef __linux__
#  include <fire/platform/linux.hpp>

namespace fire {

InotifyWatcher::InotifyWatcher(const std::string& path)
    : rootPath(path) {
  this->fd = inotify_init1(IN_NONBLOCK);
  if (this->fd < 0) {
    std::cerr << "inotify init failed" << std::endl;
    exit(EXIT_FAILURE);
  }
  const auto include_dir = Parser::get_array<std::string>("include_dir", "build");

  // check if directories to watch are specified, if not watch the root directory
  if (include_dir->empty())
    this->addWatchRecursively(path);
  // else watch the specified directories
  else
    for (const auto& dir : *include_dir)
      this->addWatchRecursively(dir);
}

InotifyWatcher::~InotifyWatcher() {
  for (const auto& pair : this->watchDescriptors) {
    inotify_rm_watch(fd, pair.first);
  }
  close(this->fd);
}

void InotifyWatcher::addWatch(const std::string& path) {
  int wd = inotify_add_watch(this->fd, path.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE | IN_MOVE);
  if (wd == -1) {
    std::cerr << "Cannot watch '" << path << "': " << strerror(errno) << std::endl;
  } else {
    this->watchDescriptors[wd] = path;
  }
}

void InotifyWatcher::removeWatch(int wd) {
  if (inotify_rm_watch(this->fd, wd) == -1) {
    std::cerr << "Error removing watch: " << strerror(errno) << std::endl;
  } else {
    this->watchDescriptors.erase(wd);
  }
}

void InotifyWatcher::watch() {
  while (true) {
    this->startWatch();
  }
}

void InotifyWatcher::startWatch() {
  char buffer[4096] __attribute__((aligned(__alignof__(struct inotify_event))));
  const struct inotify_event* event;
  ssize_t length;

  while ((length = read(this->fd, buffer, sizeof buffer)) > 0) {
    for (char* ptr = buffer; ptr < buffer + length; ptr += sizeof(struct inotify_event) + event->len) {
      event = (const struct inotify_event*) ptr;
      this->handleEvent(event);
    }
  }

  if (length < 0 && errno != EAGAIN) {
    std::cerr << "read error: " << strerror(errno) << std::endl;
  }
}

void InotifyWatcher::handleEvent(const struct inotify_event* event) {
  auto now = std::chrono::steady_clock::now();
  if (event->len) {
    std::string path = this->watchDescriptors[event->wd] + '/' + event->name;

    if (event->mask & IN_CREATE) {
      std::cout << "Created: " << path << std::endl;
      if (event->mask & IN_ISDIR) {
        this->addWatchRecursively(path);
      }
    } else if (event->mask & IN_DELETE) {
      std::cout << "Deleted: " << path << std::endl;
      if (event->mask & IN_ISDIR) {
        this->removeWatch(event->wd);
      }
    } else if (event->mask & IN_MODIFY) {
      if (this->isValidFile(event->name)) {
        if (this->lastModified.find(path) != this->lastModified.end() &&
            std::chrono::duration_cast<std::chrono::milliseconds>(now - this->lastModified[path]).count() < 100) {
          return;
        }
        this->lastModified[path] = now;
        std::cout << "Modified: " << path << std::endl;
      }
    } else if (event->mask & IN_MOVED_FROM || event->mask & IN_MOVED_TO) {
      std::cout << "Moved: " << path << std::endl;
      if (event->mask & IN_ISDIR) {
        this->removeWatch(event->wd);
      }
    }
  }
}

};  // namespace fire

#endif