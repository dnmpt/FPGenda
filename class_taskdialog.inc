{
               FPGenda - O código da tua produtividade.
               
                        versão 0.5 / 20251103
                        
    Copyright (c) 2025 by Duarte Mendes.
    
         >>> INCLUDE FILE : class_taskdialog.inc <<<
    
    Part of FPGenda software build.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Descrição:
      Diálogo de tarefa (usando EditDate textual)
}


type
  TTaskDialog = class(TfpgForm)
  private
    lblPri, lblInicio, lblFim, lblNome, lblProj: TfpgLabel;
    cbPri: TfpgComboBox;
    edtInicio: TfpgEdit;     // texto AAAAMMDD
    edtFim: TfpgEdit;     // texto AAAAMMDD
    edtNome: TfpgEdit;
    edtProj: TfpgEdit;
    chkTop: TfpgCheckBox;
    chkDone: TfpgCheckBox;
    btnOK, btnCancel: TfpgButton;
    FValidationFailed: Boolean;
    
    // métodos internos de sincronização
    function GetPrioridade: Integer;
    procedure SetPrioridade(AValue: Integer);
    function GetInicio: string;
    procedure SetInicio(const AValue: string);
    function GetFim: string;
    procedure SetFim(const AValue: string);
    function GetNome: string;
    procedure SetNome(const AValue: string);
    function GetProj: string;
    procedure SetProj(const AValue: string);
    function GetImediata: Boolean;
    procedure SetImediata(AValue: Boolean);
    function GetConcluida: Boolean;
    procedure SetConcluida(AValue: Boolean);
    
    procedure OnClick(Sender: TObject);
    procedure OnCancel(Sender: TObject);
     
  public
  
    constructor Create(AOwner: TComponent); override;
    function Execute(var APri: Integer; var AInicio, AFim, ANome, Aproj: String; var ATop, ADone: Boolean): Boolean;
    
    // as tuas propriedades expostas ao exterior
    property ValidationFailed: Boolean read FValidationFailed;
    property Prioridade: Integer read GetPrioridade write SetPrioridade;
    property Inicio: string read GetInicio write SetInicio;
    property Fim: string read GetFim write SetFim;
    property Nome: string read GetNome write SetNome;
    property Projecto: string read GetProj write SetProj;
    property Imediata: Boolean read GetImediata write SetImediata;
    property Concluida: Boolean read GetConcluida write SetConcluida;
    
  end;

    // Getters/Setters
    function TTaskDialog.GetPrioridade: Integer;
    begin
      Result := cbPri.FocusItem + 1;
      if Result < 1 then Result := 1;
    end;

    procedure TTaskDialog.SetPrioridade(AValue: Integer);
    var idx: Integer;
    begin
      idx := AValue - 1;
      if idx < 0 then idx := 0;
      if (cbPri.Items <> nil) and (idx >= cbPri.Items.Count) then idx := cbPri.Items.Count-1;
      cbPri.FocusItem := idx;
    end;
    
    //Data de Inicio
    function TTaskDialog.GetInicio: string;
    begin
      Result := edtInicio.Text;
    end;

    procedure TTaskDialog.SetInicio(const AValue: string);
    begin
      edtInicio.Text := AValue;
    end;

    //Data de Fim
    function TTaskDialog.GetFim: string;
    begin
      Result := edtFim.Text;
    end;

    procedure TTaskDialog.SetFim(const AValue: string);
    begin
      edtFim.Text := AValue;
    end;

    //Nome da Tarefa
    function TTaskDialog.GetNome: string;
    begin
      Result := edtNome.Text;
    end;

    procedure TTaskDialog.SetNome(const AValue: string);
    begin
      edtNome.Text := AValue;
    end;

    //Nome do Projecto
    function TTaskDialog.GetProj: string;
    begin
      Result := edtProj.Text;
    end;

    procedure TTaskDialog.SetProj(const AValue: string);
    begin
      edtProj.Text := AValue;
    end;
    
    //Imediata
    function TTaskDialog.GetImediata: Boolean;
    begin
      Result := chkTop.Checked;
    end;

    procedure TTaskDialog.SetImediata(AValue: Boolean);
    begin
      chkTop.Checked := AValue;
    end;
    
    //Concluida
    function TTaskDialog.GetConcluida: Boolean;
    begin
      Result := chkDone.Checked;
    end;

    procedure TTaskDialog.SetConcluida(AValue: Boolean);
    begin
      chkDone.Checked := AValue;
    end;


// On clicks OK e CANCEL
procedure TTaskDialog.OnClick(Sender: TObject);
begin
  ModalResult := TfpgModalResult(1); // OK
end;


procedure TTaskDialog.OnCancel(Sender: TObject);
begin
  ModalResult := TfpgModalResult(2); // Cancel
end;


constructor TTaskDialog.Create(AOwner: TComponent);
var i: Integer;
    
begin
  inherited Create(AOwner);
  WindowPosition := wpOneThirdDown; //Segue regra do parent - visualmente mais confortável 
  SetPosition(450, 400, 420, 230); 
  WindowTitle := 'Tarefa';

  lblPri := TfpgLabel.Create(self);
  lblPri.Text := 'Prioridade (1-5):';
  lblPri.Left := 16; lblPri.Top := 16;

  cbPri := TfpgComboBox.Create(self);
  cbPri.Parent := self;
  cbPri.Left := 16; cbPri.Top := 36; cbPri.Width := 120;
  for i := 1 to 5 do cbPri.Items.Add(IntToStr(i));
  cbPri.DropDownCount := 4;

  lblInicio := TfpgLabel.Create(self);
  lblInicio.Text := 'Início (AAAAMMDD):';
  lblInicio.Left := 160; lblInicio.Top := 16;
  lblInicio.Width := 110;

  edtInicio := TfpgEdit.Create(self);
  edtInicio.Parent := self;
  edtInicio.Left := 160; edtInicio.Top := 36; edtInicio.Width := 110;
  edtInicio.MaxLength := 8;

  lblFim := TfpgLabel.Create(self);
  lblFim.Text := 'Fim (AAAAMMDD):';
  lblFim.Left := 294; lblFim.Top := 16;
  lblFim.Width := 110;

  edtFim := TfpgEdit.Create(self);
  edtFim.Parent := self;
  edtFim.Left := 294; edtFim.Top := 36; edtFim.Width := 110;
  edtFim.MaxLength := 8;

  lblNome := TfpgLabel.Create(self);
  lblNome.Text := 'Nome da tarefa:';
  lblNome.Left := 16; lblNome.Top := 70;

  edtNome := TfpgEdit.Create(self);
  edtNome.Parent := self;
  edtNome.Left := 16; edtNome.Top := 90; edtNome.Width := 384;

  lblProj := TfpgLabel.Create(self);
  lblProj.Text := 'Nome do Projeto:';
  lblProj.Left := 16; lblProj.Top := 120;

  edtProj := TfpgEdit.Create(self);
  edtProj.Parent := self;
  edtProj.Left := 16; edtProj.Top := 140; edtProj.Width := 384;

  chkTop := TfpgCheckBox.Create(self);
  chkTop.Parent := self;
  chkTop.Text := 'Imediata';
  chkTop.Left := 16; chkTop.Top := 170;

  chkDone := TfpgCheckBox.Create(self);
  chkDone.Parent := self;
  chkDone.Text := 'Concluída';
  chkDone.Left := 90; chkDone.Top := 170;

  btnOK := TfpgButton.Create(self);
  btnOK.Parent := self;
  btnOK.Text := 'OK';
  btnOK.Left := 240; btnOK.Top := 190;
  btnOK.OnClick := @Onclick;


  btnCancel := TfpgButton.Create(self);
  btnCancel.Parent := self;
  btnCancel.Text := 'Cancelar';
  btnCancel.Left := 320; btnCancel.Top := 190;
  btnCancel.OnClick := @OnCancel;
End;

function TTaskDialog.Execute(var APri: Integer; var AInicio, AFim, ANome, AProj: String; var ATop, ADone: Boolean): Boolean;

begin
  FValidationFailed := False;  // <- limpa sempre

  // inicializar controlos via propriedades
  Self.Prioridade := APri;
  Self.Inicio     := AInicio;
  Self.Fim        := AFim;
  Self.Nome       := ANome;
  Self.Projecto   := AProj;
  Self.Imediata   := ATop;
  Self.Concluida  := ADone;
  
  // mostrar
  if ShowModal <> TfpgModalResult(1) then
    Exit(False);  // aqui é CANCEL/ESC/fechar janela
    
  // recolher de volta
  APri    := Self.Prioridade;
  ANome := Trim(Self.Nome);
  AProj := Trim(Self.Projecto);
  ATop  := Self.Imediata;
  ADone := Self.Concluida;
  AInicio := OnlyDigits(Trim(Self.Inicio));
  AFim    := OnlyDigits(Trim(Self.Fim));
  
  // validação
  if (AInicio = '') or (not IsYYYYMMDD(AInicio)) then
  begin
    ShowMessage('Data Inicial vazia ou inválida. Use AAAAMMDD.', 'Aviso');
    FValidationFailed := True;
    Exit(False);
  end;

  if (AFim <> '') and (not IsYYYYMMDD(AFim)) then
  begin
    ShowMessage('Data Final inválida. Use AAAAMMDD ou deixe vazio.', 'Aviso');
    FValidationFailed := True;
    Exit(False);
  end;

  if (AFim <> '') and (AFim < AInicio) then
  begin
    ShowMessage('Data Final anterior à Data Inicial.', 'Aviso');
    FValidationFailed := True;
    Exit(False);
  end;

  // Se FORM VALIDADO (Datas OK)
  Result := True;
end;
