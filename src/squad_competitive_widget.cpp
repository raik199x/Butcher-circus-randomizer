#include "squad_competitive_widget.hpp"

SquadCompetitiveWidget::SquadCompetitiveWidget(const Fighter &fighter, uint8_t position, const bool mirrored)
    : SquadAbstractWidget(fighter) {
  if (mirrored) {
    position = kRequiredNumberOfFighters + 1 - position;
  }
  this->fighter_pos.setText("Pos " + QString::number(position));
  this->resizePixmapInitLayouts();
  mirrored ? this->mirroredLayoutItems() : this->layoutItems();
}

void SquadCompetitiveWidget::resizePixmapInitLayouts() {
  this->setLayout(&this->main_layout);
  this->fighter_label.setPixmap(this->fighter_label.pixmap().scaled(this->kFighterImageSize));

  // init skills layout
  for (uint8_t row = 0; row < kRequiredSkillsForFighter / 2; row++) {
    for (uint8_t column = 0; column < kRequiredSkillsForFighter / 2; column++) {
      const uint8_t label_index = row * (kRequiredSkillsForFighter / 2) + column;
      this->skill_labels[label_index].setPixmap(this->skill_labels[label_index].pixmap().scaled(this->kSkillImageSize));
      this->skills_layout.addWidget(&this->skill_labels[label_index], row, column);
    }
  }

  // init trinkets
  for (auto &trinket_label : this->trinket_labels) {
    trinket_label.setPixmap(trinket_label.pixmap().scaled(this->kTrinketImageSize));
    this->trinkets_layout.addWidget(&trinket_label);
  }

  this->hero_with_pos_layout.addWidget(&this->fighter_label);
  this->hero_with_pos_layout.addWidget(&this->fighter_pos);
}

void SquadCompetitiveWidget::layoutItems() {
  main_layout.addLayout(&trinkets_layout);
  main_layout.addLayout(&this->hero_with_pos_layout);
  main_layout.addLayout(&this->skills_layout);
}

void SquadCompetitiveWidget::mirroredLayoutItems() {
  this->fighter_label.setPixmap(this->fighter_label.pixmap().transformed(this->mirroring));
  main_layout.addLayout(&this->skills_layout);
  main_layout.addLayout(&this->hero_with_pos_layout);
  main_layout.addLayout(&trinkets_layout);
}