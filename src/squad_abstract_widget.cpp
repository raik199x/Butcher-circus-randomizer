#include "squad_abstract_widget.hpp"

SquadAbstractWidget::SquadAbstractWidget(const Fighter &fighter) {
  this->setFighter(fighter);
}

void SquadAbstractWidget::setFighterImage(const QString &fighter_name) {
  this->fighter_label.setPixmap(QPixmap(kQtAssetHeroesWithSpellsIndex + fighter_name + kQtAssetHeroPrefix +
                                        fighter_name + kQtAssetImageTypePostfix));
}

void SquadAbstractWidget::setSkillsToNA() {
  for (auto &skills_iter : this->skill_labels) {
    skills_iter.setPixmap(QPixmap(kQtAssetNAImage));
  }
}

void SquadAbstractWidget::setSkillsImages(const QString &fighter_name, const QString &skills) {
  if (fighter_name == kFighterWithAllSkills) {
    this->setSkillsToNA();
    return;
  }

  for (int64_t skills_iter = 0; skills_iter < skills.size(); skills_iter++) {
    this->skill_labels[skills_iter].setPixmap(
        QPixmap(kQtAssetHeroesWithSpellsIndex + fighter_name + "/" + skills[skills_iter] + kQtAssetImageTypePostfix));
  }
}

void SquadAbstractWidget::setTrinketsImages(const std::array<QString, kMaxTrinketAmountForFighter> &trinkets) {
  for (size_t trinket_iter = 0; trinket_iter < trinkets.size(); trinket_iter++) {
    this->trinket_labels[trinket_iter].setPixmap(
        QPixmap(kQtAssetTrinketIndex + trinkets[trinket_iter] + kQtAssetImageTypePostfix));
  }
}

void SquadAbstractWidget::setFighter(const Fighter &fighter) {
  this->setFighterImage(fighter.name);
  this->setSkillsImages(fighter.name, fighter.skills);
  this->setTrinketsImages(fighter.trinkets);
}
