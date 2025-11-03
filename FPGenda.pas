{
               FPGenda - O código da tua produtividade.
               
                        versão 0.5 / 20251103
                        
    Copyright (c) 2025 by Duarte Mendes.
    
    >>> Compilar com Free Pascal 3.2.2 + fpGUI 1.5 Devel

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Descrição:
      A tua agenda moderna, precisa e programável.
          - Leve e portátil (sem dependências externas);
          - Organizada como um programador pensa;
          - Visual como um gestor precisa;
          - E fluida como uma agenda pessoal deve ser.
}


program RetroTaskManagerGUI;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  SysUtils, Classes,typinfo,
  fpg_main, fpg_form, fpg_menu, fpg_grid,
  fpg_button, fpg_label, fpg_edit, fpg_combobox,
  fpg_checkbox, fpg_dialogs, fpg_base,fpg_memo,
  myfpg_utils, sqlite3conn, sqldb;


//Classe TASK
{$I 'class_task.inc'}


//DIALOG DAS TASKS  
{$I 'class_taskdialog.inc'}


//MAIN FORM  
{$I 'class_mainform.inc'}


//NOTE
var ActiveTaskName : String;
{$I 'class_note.inc'}


//CONTROLLER  
{$I 'controller_mainform.inc'}


var
  frm: TMainForm;
  app: TfpgApplication;
    
begin
  app:=fpgApplication;
  // Fonte com bom coverage Unicode
  //app.DefaultFont := 'DejaVu Sans';
  app.Initialize;
  frm := TMainForm.Create(nil);
  frm.Show;
  fpgApplication.Run;
  frm.Free;
end.
