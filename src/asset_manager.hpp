#pragma once

#include <QString>
#include <QRegularExpression>

#include <vector>

struct TrinketInfo {
  QString name;
  QString hero_restriction;
  uint8_t level_restriction;
};

const QString kQtAssetPositionEmptyCircle   = ":/Position/circles/emptyCircle.png";
const QString kQtAssetPositionFullCircle    = ":/Position/circles/fullCircle.png";
const QString kQtAssetTrinketIndex          = ":/trinkets/trinkets/";
const QString kQtAssetTrinketList           = kQtAssetTrinketIndex + "list.txt";
const QString kQtAssetHeroesWithSpellsIndex = ":/heroes/heroes+spells/";
const QString kQtAssetHeroPrefix            = "/hero_";
const QString kQtAssetImageTypePostfix      = ".png";

class AssetManagerSingletone {
public:
  // Singletone prevents copying
  AssetManagerSingletone(const AssetManagerSingletone &)            = delete;
  AssetManagerSingletone &operator=(const AssetManagerSingletone &) = delete;

  static AssetManagerSingletone         &getInstance();
  [[nodiscard]] std::vector<TrinketInfo> getTrinkets() const;
  [[nodiscard]] bool                     isTrinketsParsed() const;
  bool                                   loadTrinkets(const QString &path_to_file);

  const QString  kTrinketWithoutHeroLimit = "NO_HERO_LIMIT";
  static QString parseTrinketName(const QString &line);
  QString        parseTrinketHeroLimit(const QString &line);

private:
  AssetManagerSingletone(){};
  ~AssetManagerSingletone() {
  }

  std::vector<TrinketInfo> trinkets;
};