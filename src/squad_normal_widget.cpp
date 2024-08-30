#include "squad_normal_widget.hpp"

SquadNormalWidget::SquadNormalWidget(const Fighter &fighter, uint8_t position, const bool mirrored)
    : SquadAbstractWidget(fighter) {
  if(mirrored){
    position = this->fighter_position_labels.size() + 1 - position;
  }
  this->setFighterPosition(position);
  this->resizePixmapInitLayouts();

  mirrored ? this->mirroredLayoutItems() : this->layoutItems();
}

void SquadNormalWidget::setFighterPosition(const uint8_t position) {
  for (size_t iter = 0; iter < this->fighter_position_labels.size(); iter++) {
    this->fighter_position_labels[iter].setPixmap(position - 1 == iter ? QPixmap(kQtAssetPositionFullCircle)
                                                                       : QPixmap(kQtAssetPositionEmptyCircle));
  }
}

void SquadNormalWidget::resizePixmapInitLayouts() {
  this->setLayout(&this->main_layout);
  this->fighter_label.setPixmap(this->fighter_label.pixmap().scaled(this->kFighterImageSize));

  // init skills layout
  for (auto &skill_label : this->skill_labels) {
    skill_label.setPixmap(skill_label.pixmap().scaled(this->kSkillImageSize));
    // skill_label.setMinimumSize(this->kSkillImageSize);
    skills_layout.addWidget(&skill_label);
  }

  // init trinkets
  for (auto &trinket_label : this->trinket_labels) {
    trinket_label.setPixmap(trinket_label.pixmap().scaled(this->kTrinketSize));
    // trinket_label.setMinimumSize(this->trinket_size);
    this->trinkets_layout.addWidget(&trinket_label);
  }

  // init position
  for (auto &position_label : this->fighter_position_labels) {
    position_layout.addWidget(&position_label);
  }
}

void SquadNormalWidget::layoutItems() {
  main_layout.addLayout(&trinkets_layout);

  hero_with_pos_layout.addWidget(&this->fighter_label);
  hero_with_pos_layout.addLayout(&position_layout);

  skills_hero_pos_layout.addLayout(&hero_with_pos_layout);
  skills_hero_pos_layout.addLayout(&skills_layout);

  main_layout.addLayout(&skills_hero_pos_layout);
}

void SquadNormalWidget::mirroredLayoutItems() {
  this->fighter_label.setPixmap(this->fighter_label.pixmap().transformed(this->mirroring));

  hero_with_pos_layout.addLayout(&position_layout);
  hero_with_pos_layout.addWidget(&this->fighter_label);

  skills_hero_pos_layout.addLayout(&hero_with_pos_layout);
  skills_hero_pos_layout.addLayout(&skills_layout);

  main_layout.addLayout(&skills_hero_pos_layout);

  main_layout.addLayout(&trinkets_layout);
}