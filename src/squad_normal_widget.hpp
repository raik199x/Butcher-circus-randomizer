#pragma once

#include <array>

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "random_master.hpp"

class SquadNormalWidget : public QWidget {
  Q_OBJECT
private slots:

public:
  explicit SquadNormalWidget(const Fighter &fighter, bool mirroring_required = false, uint8_t position = 1);
  ~SquadNormalWidget();

  SquadNormalWidget(const SquadNormalWidget &other)            = delete;
  SquadNormalWidget &operator=(const SquadNormalWidget &other) = delete;
  SquadNormalWidget(SquadNormalWidget &&other)                 = delete;
  SquadNormalWidget &operator=(SquadNormalWidget &&other)      = delete;

private:
  QHBoxLayout main_layout;

  QVBoxLayout skills_hero_pos_layout;
  QHBoxLayout hero_with_pos_layout;

  QHBoxLayout trinkets_layout;
  QHBoxLayout skills_layout;
  QHBoxLayout position_layout;

  std::array<QLabel, kMaxTrinketAmountForFighter> trinket_labels;
  std::array<QLabel, kRequiredNumberOfFighters>   fighter_position_labels;
  std::array<QLabel, kRequiredSkillsForFighter>   skill_label;
  QLabel                                          fighter_label;

  const QSize kHeroImageSize  = QSize(60, 60);
  const QSize kSkillImageSize = QSize(60, 60);
  const QSize trinket_size    = QSize(50, 100);
};