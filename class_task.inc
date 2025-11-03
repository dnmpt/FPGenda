{
               FPGenda - O código da tua produtividade.
               
                        versão 0.5 / 20251103
                        
    Copyright (c) 2025 by Duarte Mendes.
    
              >>> INCLUDE FILE : class.inc <<<
    
    Part of FPGenda software build.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Descrição:
      CLASSE DAS TAREFAS
}

type
  { === Modelo === }
  TTask = class
  private
    FID: Integer;
    FPri: Integer;
    FInicio: String; // AAAAMMDD - Início
    FFim: String;   // AAAAMMDD - Fim
    FNome: String;
    FDone: Boolean;
    FTop: Boolean; //Prioridade max
    FProj: String; //Projecto
    FNote: String;
  public
    property ID: Integer read FID write FID;
    property Prioridade: Integer read FPri write FPri;
    property Inicio: String read FInicio write FInicio;
    property Fim: String read FFim write FFim;
    property Nome: String read FNome write FNome;
    property Concluida: Boolean read FDone write FDone;
    property PrioMax: Boolean read FTop write FTop;
    property Projecto: String read FProj write FProj;
    property Notas: String read FNote write FNote;
  end;

  TTaskList = class
  private
    FList: TList;
    function GetCount: Integer;
    function GetItem(Index: Integer): TTask;
    function NextID: Integer;
  public
    constructor Create;
    destructor Destroy; override;

    procedure Clear;
    procedure Add(const APri: Integer; const AInicio, AFim, ANome, AProj: String; ATop, ADone: Boolean; ANotas: String);
    procedure AddTaskObj(ATask: TTask);
    function  FindByID(const AID: Integer): Integer; // idx ou -1
    procedure DeleteByIndex(const Idx: Integer);
    procedure ToggleTopByIndex(const Idx: Integer);
    procedure ToggleDoneByIndex(const Idx: Integer);

    procedure ExportCSV(const FileName: String);

    property Count: Integer read GetCount;
    property Items[Index: Integer]: TTask read GetItem; default;
  end;

  
 { === TTaskList === }

constructor TTaskList.Create;
begin
  inherited Create;
  FList := TList.Create;
end;

destructor TTaskList.Destroy;
begin
  Clear;
  FList.Free;
  inherited Destroy;
end;

procedure TTaskList.Clear;
var i: Integer;
begin
  for i := 0 to FList.Count-1 do TObject(FList[i]).Free;
  FList.Clear;
end;

function TTaskList.GetCount: Integer;
begin
  Result := FList.Count;
end;

function TTaskList.GetItem(Index: Integer): TTask;
begin
  Result := TTask(FList[Index]);
end;

function TTaskList.NextID: Integer;
begin
  if Count = 0 then Result := 1
  else Result := Items[Count-1].ID + 1;
end;

procedure TTaskList.Add(const APri: Integer; const AInicio, AFim, ANome, AProj: String; ATop, ADone: Boolean; ANotas: String);
var t: TTask;
begin
  t := TTask.Create;
  t.ID := NextID;
  t.Prioridade := APri;
  t.Inicio := AInicio;
  t.Fim := AFim;
  t.Nome := ANome;
  t.PrioMax := ATop;
  t.Concluida := ADone;
  t.Projecto := AProj;
  t.Notas := ANotas;
  FList.Add(t);
end;


function TTaskList.FindByID(const AID: Integer): Integer;
var i: Integer;
begin
  for i := 0 to Count-1 do
    if Items[i].ID = AID then Exit(i);
  Result := -1;
end;

procedure TTaskList.DeleteByIndex(const Idx: Integer);
begin
  if (Idx >= 0) and (Idx < Count) then
  begin
    TObject(FList[Idx]).Free;
    FList.Delete(Idx);
  end;
end;


procedure TTaskList.ToggleTopByIndex(const Idx: Integer);
begin
  if (Idx >= 0) and (Idx < Count) then
    Items[Idx].PrioMax := not Items[Idx].PrioMax;
end;


procedure TTaskList.ToggleDoneByIndex(const Idx: Integer);
begin
  if (Idx >= 0) and (Idx < Count) then
    Items[Idx].Concluida := not Items[Idx].Concluida;
end;


procedure TTaskList.AddTaskObj(ATask: TTask);
begin
  FList.Add(ATask);
end;


procedure TTaskList.ExportCSV(const FileName: String);
var
  f: TextFile; i: Integer; t: TTask;
begin
  AssignFile(f, FileName); Rewrite(f);
  try
    WriteLn(f, 'id,pri,inicio,fim,nome,proj,top,done,notas');
    for i := 0 to Count-1 do
    begin
      t := Items[i];
      WriteLn(f, Format('%d,%d,%s,%s,%s,%s,%d,%d,%s',
        [t.ID, t.Prioridade, t.Inicio, t.Fim, t.Nome, t.Projecto, Ord(t.PrioMax), Ord(t.Concluida), t.Notas]));
    end;
  finally
    CloseFile(f);
  end;
end;
 
