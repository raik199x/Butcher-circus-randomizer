#include "squad_abstract_widget.hpp"

SquadAbstractWidget::SquadAbstractWidget(const Fighter &fighter) {
  this->setFighter(fighter);
}

/**
 * @brief Inits fighter pixmap.
 *
 * @param fighter_name name that is used to find image in assets.
 */
void SquadAbstractWidget::setFighterImage(const QString &fighter_name) {
  this->fighter_label.setPixmap(QPixmap(kQtAssetHeroesWithSpellsIndex + fighter_name + kQtAssetHeroPrefix +
                                        fighter_name + kQtAssetImageTypePostfix));
}

/**
 * @brief Sets all skills to "Not Announced" image
 *
 */
void SquadAbstractWidget::setSkillsToNA() {
  for (auto &skills_iter : this->skill_labels) {
    skills_iter.setPixmap(QPixmap(kQtAssetNAImage));
  }
}

/**
 * @brief Inits skills pixmap
 *
 * @param fighter_name fighter that is stored in assets with his skills images
 * @param skills line containing numbers of skills
 */
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

/**
 * @brief Inits pixmaps for trinkets.
 *
 * @param trinkets Names of trinkets that stored in assets
 */
void SquadAbstractWidget::setTrinketsImages(const std::array<QString, kMaxTrinketAmountForFighter> &trinkets) {
  for (size_t trinket_iter = 0; trinket_iter < trinkets.size(); trinket_iter++) {
    this->trinket_labels[trinket_iter].setPixmap(
        QPixmap(kQtAssetTrinketIndex + trinkets[trinket_iter] + kQtAssetImageTypePostfix));
  }
}

/**
 * @brief Helper function to perform all operations required to init pixmaps for fighter
 *
 * @param fighter struct containing all info about fighter
 */
void SquadAbstractWidget::setFighter(const Fighter &fighter) {
  this->setFighterImage(fighter.name);
  this->setSkillsImages(fighter.name, fighter.skills);
  this->setTrinketsImages(fighter.trinkets);
}
