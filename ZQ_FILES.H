//--------------------------------------------------------
//  Zelda Classic
//  by Jeremy Craner, 1999-2000
//
//  zq_files.cc
//
//  File support for ZQuest.
//
//--------------------------------------------------------

#ifndef _ZQ_FILES_H_
#define _ZQ_FILES_H_

#include "zc_alleg.h"

extern char qtbuf[31];

void reset_qt(int index);
void init_qts();
void edit_qt();                                             //this is used to set the quest template for the current quest
void edit_qt(int index);
char *qtlist(int index, int *list_size);
int qtlist_del();
int ListQTs(bool edit);
int onQuestTemplates();
int NewQuestFile();
int onNew();
int onSave();
int onSaveAs();
int onOpen();
int onRevert();
int get_import_map_bias();
int onImport_Map();
int onExport_Map();
int onImport_DMaps();
int onExport_DMaps();
int onImport_Pals();
int onExport_Pals();
int onImport_Msgs();
int onExport_Msgs();
int onImport_Combos();
int onExport_Combos();
int onImport_Tiles();
int onExport_Tiles();
int onImport_ZGP();
int onExport_ZGP();
int onImport_ZQT();
int onExport_ZQT();
int onImport_UnencodedQuest();
int onExport_UnencodedQuest();
void center_zq_files_dialogs();
#endif
