#pragma once

#include <QBoxLayout>

#include "squad_abstract_widget.hpp"
#include "config.h"

class SquadNormalWidget : public SquadAbstractWidget {
  Q_OBJECT
private slots:

public:
  SquadNormalWidget(const Fighter &fighter, uint8_t position, bool mirrored);
  virtual ~SquadNormalWidget() = default;

  SquadNormalWidget(const SquadNormalWidget &other)             = delete;
  SquadNormalWidget &operator=(const SquadNormalWidget &other)  = delete;
  SquadNormalWidget(const SquadNormalWidget &&other)            = delete;
  SquadNormalWidget &operator=(const SquadNormalWidget &&other) = delete;

private:
  void resizePixmapInitLayouts();

  void layoutItems();
  void mirroredLayoutItems();

  void setFighterPosition(uint8_t position);

  // Layouts
  QHBoxLayout main_layout;

  QVBoxLayout skills_hero_pos_layout;
  QHBoxLayout hero_with_pos_layout;

  QHBoxLayout trinkets_layout;
  QHBoxLayout skills_layout;
  QHBoxLayout position_layout;

  // Widgets
  std::array<QLabel, kRequiredNumberOfFighters> fighter_position_labels;

  // sizes
  const QSize kFighterImageSize  = QSize(60, 60);
  const QSize kSkillImageSize = QSize(60, 60);
  const QSize kTrinketSize    = QSize(50, 100);
};