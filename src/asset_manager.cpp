#include <QFile>
#include <QTextStream>

#include "asset_manager.hpp"

/**
 * @file asset_manager.cpp
 * @brief Class for manipulating assets that indexed by Qt
 *
 */

AssetManagerSingletone &AssetManagerSingletone::getInstance() {
  static AssetManagerSingletone instance;
  return instance;
}

std::vector<TrinketInfo> AssetManagerSingletone::getTrinkets() const {
  return this->trinkets;
}

bool AssetManagerSingletone::isTrinketsParsed() const {
  return !this->trinkets.empty();
}

QString AssetManagerSingletone::parseTrinketName(const QString &line) {
  qint64 open_bracket_pos = line.indexOf('[');
  if (open_bracket_pos == -1) {
    return line;
  }
  return line.mid(0, open_bracket_pos);
}

QString AssetManagerSingletone::parseTrinketHeroLimit(const QString &line) {
  qint64 open_bracket_pos = line.indexOf('[');
  if (open_bracket_pos == -1) {
    return this->kTrinketWithoutHeroLimit;
  }

  qint64 closing_bracket_pos = line.indexOf(']');
  return line.mid(open_bracket_pos + 1, closing_bracket_pos - open_bracket_pos - 1);
}

bool AssetManagerSingletone::loadTrinkets(const QString &path_to_file) {
  QFile trinket_list(path_to_file);
  if (!trinket_list.open(QIODevice::ReadOnly)) {
    return false;
  }

  QTextStream file_stream_read(&trinket_list);
  QString     line_from_file;
  uint8_t     current_level_restriction = 0;
  while (!file_stream_read.atEnd()) {
    line_from_file = file_stream_read.readLine();

    // Checking if line signalizing of new level restriction
    bool    is_number;
    uint8_t number = line_from_file.toUShort(&is_number);
    if (is_number) {
      current_level_restriction = number;
      continue;
    }

    // Preparing struct
    TrinketInfo info;
    info.level_restriction = current_level_restriction;
    info.hero_restriction  = this->parseTrinketHeroLimit(line_from_file);
    info.name              = AssetManagerSingletone::parseTrinketName(line_from_file);

    this->trinkets.emplace_back(info);
  }

  return true;
}
