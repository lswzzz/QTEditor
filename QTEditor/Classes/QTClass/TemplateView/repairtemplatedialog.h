#ifndef REPAIRTEMPLATEDIALOG_H
#define REPAIRTEMPLATEDIALOG_H

#include <QDialog>
#include "ImageSprite.h"

namespace Ui {
class RepairTemplateDialog;
}

class RepairTemplateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RepairTemplateDialog(QWidget *parent = 0);
    ~RepairTemplateDialog();
private slots:
	void on_cancelButton_clicked();
	void on_sureButton_clicked();
private:
	void createTemplate(ImageSprite* sprite);
	QVariantMap createBaseProperties(ImageSprite* sprite);
	QVariantMap createBoxProperties(ImageSprite* sprite);
	QVariantMap createMoveProperties(ImageSprite* sprite);
	QVariantMap createCircleProperties(ImageSprite* sprite);
	QVariantMap createAnimationProperties(ImageSprite* sprite);
	QVariantMap createEntityProperties(ImageSprite* sprite);
	QVariantMap createMonsterProperties(ImageSprite* sprite);
	QVariantMap createMonsterDropProperties(ImageSprite* sprite);
	QVariantMap createAiProperties(ImageSprite* sprite);
	QVariantMap createSimplerTriggerProperties(ImageSprite* sprite);
	void addPropertiesToMap(QVariantMap& map, ImageSprite* sprite);
	void addSpritesToTemplate(QString templateName);
private:
    Ui::RepairTemplateDialog *ui;
};

#endif // REPAIRTEMPLATEDIALOG_H
