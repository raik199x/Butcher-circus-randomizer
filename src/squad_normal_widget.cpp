#include "squad_normal_widget.hpp"
#include "asset_manager.hpp"

SquadNormalWidget::SquadNormalWidget(const Fighter &fighter, const bool mirroring_required, const uint8_t position) {
  this->setLayout(&this->main_layout);

  // init fighter image
  QPixmap fighter_pixmap = QPixmap(kQtAssetHeroesWithSpellsIndex + fighter.name + kQtAssetHeroPrefix + fighter.name +
                                   kQtAssetImageTypePostfix);
  if (mirroring_required) {
    fighter_pixmap = fighter_pixmap.transformed(QTransform().scale(-1, 1));
  }
  this->fighter_label.setPixmap(fighter_pixmap);

  // init skill images
  for (int64_t skills_iter = 0; skills_iter < this->skill_label.size(); skills_iter++) {
    QPixmap skill_pixmap = QPixmap(kQtAssetHeroesWithSpellsIndex + fighter.name + "/" + fighter.skills[skills_iter] +
                                   kQtAssetImageTypePostfix);
    this->skill_label[skills_iter].setPixmap(skill_pixmap);
    this->skills_layout.addWidget(&this->skill_label[skills_iter]);
  }

  // init trinkets
  // TODO(alexander): make an array
  QPixmap trinket1_pixmap = QPixmap(kQtAssetTrinketList + fighter.trinket1 + kQtAssetImageTypePostfix);
  QPixmap trinket2_pixmap = QPixmap(kQtAssetTrinketList + fighter.trinket2 + kQtAssetImageTypePostfix);
  this->trinket_labels[0].setPixmap(trinket1_pixmap);
  this->trinket_labels[1].setPixmap(trinket2_pixmap);
  this->trinkets_layout.addWidget(this->trinket_labels.data());
  this->trinkets_layout.addWidget(&this->trinket_labels[1]);

  // init position
  int start     = mirroring_required ? kRequiredNumberOfFighters - 1 : 0;
  int end       = mirroring_required ? -1 : kRequiredNumberOfFighters;
  int increment = mirroring_required ? -1 : 1;
  for (; start != end; start += increment) {
    QPixmap circle = position - 1 == start ? QPixmap(kQtAssetPositionFullCircle) : QPixmap(kQtAssetPositionEmptyCircle);
    this->fighter_position_labels[start].setPixmap(circle);
    this->position_layout.addWidget(&this->fighter_position_labels[start]);
  }

  // Finalizing
  this->main_layout.addLayout(&this->trinkets_layout);

  if (mirroring_required) {
    this->hero_with_pos_layout.addLayout(&this->position_layout);
    this->hero_with_pos_layout.addWidget(&this->fighter_label);

  } else {
    this->hero_with_pos_layout.addWidget(&this->fighter_label);
    this->hero_with_pos_layout.addLayout(&this->position_layout);
  }

  this->skills_hero_pos_layout.addLayout(&this->hero_with_pos_layout);
  this->skills_hero_pos_layout.addLayout(&this->skills_layout);

  this->main_layout.addLayout(&this->skills_hero_pos_layout);
}

SquadNormalWidget::~SquadNormalWidget() = default;
