#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

class QQuickWidget;

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = nullptr);

signals:
    void loginSuccess();
    void loginFail(int code, const QString &message);

public slots:

    /**
     * @brief 登录按钮点击事件
     */
    void handleLoginRequest(QString username,  QString password);

    /**
     * @brief handleLoginCanceled
     * @param code
     */
    void handleLoginCanceled(int code);

protected:
         void showEvent(QShowEvent *event) override;


private:
      QQuickWidget *m_quickWidget;
};

#endif // LOGINWIDGET_H
