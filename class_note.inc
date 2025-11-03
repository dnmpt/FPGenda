{
               FPGenda - O código da tua produtividade.
               
                        versão 0.5 / 20251103
                        
    Copyright (c) 2025 by Duarte Mendes.
    
          >>> INCLUDE FILE : class_note.inc <<<
    
    Part of FPGenda software build.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Descrição:
      CLASS NOTES
}


type
  TNoteDialog = class(TfpgForm)
  private
    lblTaskName: TfpgLabel;
    note: TfpgMemo;
    btnSave: TfpgButton;
    btnQuit: TfpgButton;
    procedure btnSaveClicked(Sender: TObject);
    procedure   btnQuitClicked(Sender: TObject);
  public
    constructor Create(AOwner: TComponent); override;
  end;

{ TNoteDialog }

procedure TNoteDialog.btnSaveClicked(Sender: TObject);
begin
  ModalResult := TfpgModalResult(1); // OK
end;


procedure TNoteDialog.btnQuitClicked(Sender: TObject);
begin
  ModalResult := TfpgModalResult(2); // Cancel
end;


constructor TNoteDialog.Create(AOwner: TComponent);

begin
  inherited Create(AOwner);
  SetPosition(330, 270, 740, 400);
  WindowTitle := 'Create/Edit Note';
  WindowPosition := wpOneThirdDown;

  note := TfpgMemo.Create(self);
  with note do
  begin
    SetPosition(10, 40, 720, 350);
    Anchors := [anLeft,anRight,anTop,anBottom];
    FontDesc := '#Edit1';
    TabOrder := 0;
    UseTabs := true;
  end;

  btnSave := TfpgButton.Create(self);
  with btnSave do
  begin
    SetPosition(10, 8, 80, 24);
    Anchors := [anLeft,anTop];
    Text := 'Save';
    FontDesc := '#Label1';
    Hint := 'Exit and Save Note';
    ImageName := 'stdimg.save';
    TabOrder := 1;
    OnClick := @btnSaveClicked;
  end;
  
  lblTaskName := TfpgLabel.Create(self);
  lblTaskName.FontDesc := '#Label2'; // Each character occupies aprox. 6 px
  lblTaskName.Text := 'Tarefa: ' + ActiveTaskName; //ActiveTaskName is a global var
  lblTaskName.Left := CenterText(lblTaskName.Text, 10 + 80, 650); 
  lblTaskName.Top := 12;
  lblTaskName.width :=500;

  btnQuit := TfpgButton.Create(self);
  with btnQuit do
  begin
    SetPosition(650, 8, 80, 24);
    Anchors := [anRight,anTop];
    Text := 'Quit';
    FontDesc := '#Label1';
    Hint := 'Quit without saving';
    ImageName := 'stdimg.quit';
    TabOrder := 2;
    OnClick := @btnQuitClicked;
  end;

end;
