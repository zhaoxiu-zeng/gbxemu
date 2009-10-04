/*  VisualBoyAdvance 2
    Copyright (C) 2009  VBA development team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../gba2/cemugba.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <assert.h>

#include "../gba2/common/cdriver_sound.h"    // for dummy sound output
#include "../gba2/common/cdriver_graphics.h" // for dummy graphics output

#include "paintwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_emuGBA = NULL;
    m_emuGBA = new CEmuGBA;
    Q_ASSERT( m_emuGBA != NULL );

    m_playing = false;

    m_timer = NULL;
    m_timer = new QTimer( this );
    Q_ASSERT( m_timer != NULL );
    m_timer->setInterval( 1000 / 60 ); // TODO: find out if this is exact enough
    connect( m_timer, SIGNAL(timeout()), this, SLOT(timer_timeout()) );

    m_renderTarget = NULL;
    m_renderTarget = new PaintWidget( this );
    Q_ASSERT( m_renderTarget != NULL );
    setCentralWidget( m_renderTarget );

    m_snd = NULL;
    m_snd = new CDummyDriver_Sound();
    Q_ASSERT( m_snd != NULL );
    m_emuGBA->setDriverSound( m_snd );

    m_gfx = NULL;
//    m_gfx = new CDummyDriver_Graphics();
    m_gfx = (CDriver_Graphics *)m_renderTarget;
    Q_ASSERT( m_gfx != NULL );
    m_emuGBA->setDriverGraphics( m_gfx );
}

MainWindow::~MainWindow()
{
    on_actionUnload_ROM_triggered();

    if( m_snd != NULL ) {
        delete m_snd;
    }

    if( m_gfx != NULL ) {
        delete m_gfx;
    }

    delete m_emuGBA;

    delete ui;
}

void MainWindow::on_actionLoad_ROM_triggered()
{
    QString newFileName = QFileDialog::getOpenFileName(
            this, tr("Select ROM image to load"), m_fileName, tr("GBA ROMs (*.gba)") );
    if( newFileName.isEmpty() ) return;
    QFile rom( newFileName );
    qint64 size = rom.size();
    if( size > (32*1024*1024) ) {
        QMessageBox::critical( this, tr("Error"), tr("GBA ROM size must not exceed 32 MB.") );
        return;
    }
    rom.open( QFile::ReadOnly );
    QByteArray data = rom.readAll();
    rom.close();
    Q_ASSERT( size == data.size() );
    bool retVal = m_emuGBA->loadROM( (const u8 *const)data.constData(), (u32)size );
    if( !retVal ) {
        QMessageBox::critical( this, tr("Error"), tr("ROM loading failed.") );
        return;
    }
    m_fileName = newFileName;
    ui->actionPlay_Pause->setEnabled( true );
}

void MainWindow::on_actionUnload_ROM_triggered()
{
    if( !m_fileName.isEmpty() ) {
        m_emuGBA->closeROM();
        m_fileName.clear();
        ui->actionPlay_Pause->setEnabled( false );
    }
}

void MainWindow::timer_timeout()
{
    bool retVal = false;
    retVal = m_emuGBA->emulate();

    if( retVal == false ) {
        Q_ASSERT( false ); // debug emulator code
        QMessageBox::critical( this, tr("Error"), tr("Emulator code messed up. Pausing emulation.") );
        if( m_timer->isActive() ) {
            m_timer->stop();
        }
    }
}

void MainWindow::on_actionPlay_Pause_triggered()
{
    if( !m_fileName.isEmpty() ) {
        if( m_timer->isActive() ) {
            m_timer->stop();
            ui->actionPlay_Pause->setIcon( QIcon(":/MainWindow/play_button.png") );
        } else {
            m_timer->start();
            ui->actionPlay_Pause->setIcon( QIcon(":/MainWindow/pause_button.png") );
        }
    } else {
        Q_ASSERT( false ); // should not be able to arrive here
    }
}
