#include <fire/watcher.hpp>

namespace fire {
Watcher::Watcher() {
}
Watcher::~Watcher() {
}
bool Watcher::isValidFile(const std::string& filename) {
  // check for extension
  auto validExtensions = Parser::get_array<std::string>("include_ext", "build");
  std::string::size_type idx = filename.rfind('.') + 1;
  if (idx != std::string::npos) {
    std::string ext = filename.substr(idx);
    bool valid = std::find(validExtensions->begin(), validExtensions->end(), ext) != validExtensions->end();
    // if extension is valid, check if file is in excluded list
    if (valid) {
      auto excludedFiles = Parser::get_array<std::string>("exclude_file", "build");
      return std::find(excludedFiles->begin(), excludedFiles->end(), filename) == excludedFiles->end();
    } else
      return false;
  } else  // invalid extension
    return false;
}

bool Watcher::isValidDirectory(const std::string& path) {
  namespace fs = std::filesystem;
  auto excludedDirs = Parser::get_array<std::string>("exclude_dir", "build");
  return std::find(excludedDirs->begin(), excludedDirs->end(), fs::relative(path, fs::current_path().string())) == excludedDirs->end();
}

void Watcher::addWatchRecursively(const std::string& path) {
  std::queue<std::string> directories;
  directories.push(path);

  while (!directories.empty()) {
    std::string currentPath = directories.front();
    directories.pop();

    // skip excluded directories
    if (!this->isValidDirectory(currentPath))
      continue;

    this->addWatch(currentPath);
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

}  // namespace fire
