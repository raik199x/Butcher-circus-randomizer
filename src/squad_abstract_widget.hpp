#pragma once

#include <QWidget>
#include <QLabel>

#include "config.h"
#include "random_master.hpp"

struct FighterPixmaps {
  QPixmap                                          fighter;
  std::array<QPixmap, kRequiredSkillsForFighter>   skills;
  std::array<QPixmap, kMaxTrinketAmountForFighter> trinkets;
};

class SquadAbstractWidget : public QWidget {
private slots:

public:
  explicit SquadAbstractWidget(const Fighter &fighter);
  virtual ~SquadAbstractWidget() = default;

  SquadAbstractWidget(const SquadAbstractWidget &other)             = delete;
  SquadAbstractWidget &operator=(const SquadAbstractWidget &other)  = delete;
  SquadAbstractWidget(const SquadAbstractWidget &&other)            = delete;
  SquadAbstractWidget &operator=(const SquadAbstractWidget &&other) = delete;

protected:
  //
  QTransform mirroring = QTransform().scale(-1, 1);

  // methods
  void setSkillsToNA();
  void setFighterImage(const QString &fighter_name);
  void setSkillsImages(const QString &fighter_name, const QString &skills);
  void setTrinketsImages(const std::array<QString, kMaxTrinketAmountForFighter> &trinkets);
  void setFighter(const Fighter &fighter);

  // Widgets
  QLabel                                          fighter_label;
  std::array<QLabel, kRequiredSkillsForFighter>   skill_labels;
  std::array<QLabel, kMaxTrinketAmountForFighter> trinket_labels;
};