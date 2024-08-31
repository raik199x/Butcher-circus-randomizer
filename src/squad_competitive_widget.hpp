#pragma once

#include <QBoxLayout>

#include "squad_abstract_widget.hpp"

class SquadCompetitiveWidget : public SquadAbstractWidget {
  Q_OBJECT
private slots:
public:
  SquadCompetitiveWidget(const Fighter &fighter, uint8_t position, bool mirrored);
  virtual ~SquadCompetitiveWidget() = default;

  SquadCompetitiveWidget(const SquadCompetitiveWidget &other)             = delete;
  SquadCompetitiveWidget &operator=(const SquadCompetitiveWidget &other)  = delete;
  SquadCompetitiveWidget(const SquadCompetitiveWidget &&other)            = delete;
  SquadCompetitiveWidget &operator=(const SquadCompetitiveWidget &&other) = delete;

private:
  // methods
  void resizePixmapInitLayouts();
  void layoutItems();
  void mirroredLayoutItems();
  //
  QLabel fighter_pos;
  // Layouts
  QHBoxLayout main_layout;
  QHBoxLayout trinkets_layout;
  QGridLayout skills_layout;
  QVBoxLayout hero_with_pos_layout;
  // Sizes
  const QSize kFighterImageSize = QSize(50, 50);
  const QSize kSkillImageSize   = QSize(40, 40);
  const QSize kTrinketImageSize = QSize(40, 70);
};