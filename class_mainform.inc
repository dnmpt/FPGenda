{
               FPGenda - O código da tua produtividade.
               
                        versão 0.5 / 20251103
                        
    Copyright (c) 2025 by Duarte Mendes.
    
          >>> INCLUDE FILE : class_mainform.inc <<<
    
    Part of FPGenda software build.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Descrição:
      JANELA PRINCIPAL (MAIN FORM))
}

type
  TMainForm = class(TfpgForm)
  private
    Menu: TfpgMenuBar;
    FFileSubMenu, FTasksSubMenu, FHelpSubMenu: TfpgPopupMenu;
    Grid: TfpgStringGrid;
    Tasks: TTaskList;
    FFiltersSubMenu: TfpgPopupMenu;

    procedure BuildMenu;
    procedure RefreshGrid;

    // Ações de menu
    procedure DoNew(Sender: TObject);
    procedure DoOpen(Sender: TObject);
    procedure DoSave(Sender: TObject);
    procedure DoExport(Sender: TObject);
    procedure DoExit(Sender: TObject);

    procedure DoAddTask(Sender: TObject);
    procedure DoEditTask(Sender: TObject);
    procedure DoDeleteTask(Sender: TObject);
    procedure DoToggleTop(Sender: TObject);
    procedure DoToggleDone(Sender: TObject);
    procedure DoNoteTask(Sender: TObject);
    procedure DoAbout(Sender: TObject);
    
    //Filtros das Tarefas
    procedure DoFilterAll(Sender: TObject);
    procedure DoFilterByPriority(Sender: TObject);
    procedure DoFilterByDate(Sender: TObject);
    procedure DoFilterInProgress(Sender: TObject);
    procedure DoFilterDone(Sender: TObject);
    procedure DoFilterTop(Sender: TObject);
    
    //Refresh dos Filtros
    procedure RefreshGridFromIndex(const Idx: array of Integer);
    function  SafeDateKey(const S: string): string;
    procedure EnsureSQLiteDB(const AFileName: string; out AConn: TSQLite3Connection; out ATrans: TSQLTransaction);
  
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
  end;

constructor TMainForm.Create(AOwner: TComponent);
var 
    ClientWidth, ClientHeight : Integer;

begin
  inherited Create(AOwner);
  WindowPosition := wpOneThirdDown; //Posição mais confortável
  SetPosition(300,250, 760, 460);
  WindowTitle := 'FPGenda - O código da tua produtividade.';
  ClientWidth := 760;
  ClientHeight := 460;

  Tasks := TTaskList.Create;

  Menu := TfpgMenuBar.Create(self);
  Menu.Parent := self;
  BuildMenu;

  Grid := TfpgStringGrid.Create(self);
  Grid.Left := 8; Grid.Top := Menu.Height + 8; Grid.Width := ClientWidth - 16; Grid.Height := ClientHeight - (Menu.Height + 16);
  Grid.Anchors := [anLeft, anTop, anRight, anBottom];

  Grid.ColumnCount := 9; //Tem de ser definido antes
  Grid.Columns[0].Title := 'ID';
  Grid.Columns[1].Title := 'Prio';
  Grid.Columns[2].Title := 'Data Inicio';
  Grid.Columns[3].Title := 'Data Fim';
  Grid.Columns[4].Title := 'Nome da Tarefa';
  Grid.Columns[5].Title := 'Projecto';
  Grid.Columns[6].Title := 'TOP';
  Grid.Columns[7].Title := 'Done';
  Grid.Columns[8].Title := 'Notas';
  
  // definir larguras
  Grid.Columns[0].Width := 40;
  Grid.Columns[1].Width := 40;
  Grid.Columns[2].Width := 80;
  Grid.Columns[3].Width := 80;
  Grid.Columns[4].Width := 240;
  Grid.Columns[5].Width := 130;
  Grid.Columns[6].Width := 40;
  Grid.Columns[7].Width := 40;
  Grid.Columns[8].Width := 50;

  Grid.RowCount := 0;

  RefreshGrid;
end;

destructor TMainForm.Destroy;
begin
  Tasks.Free;
  inherited Destroy;
end;


procedure TMainForm.BuildMenu;
begin
  // Menubar
  if Menu = nil then
    Menu := TfpgMenuBar.Create(self);
  Menu.Align := alTop;
  Menu.Parent := self;

  // Create submenus
  FFileSubMenu  := TfpgPopupMenu.Create(self);
  FTasksSubMenu := TfpgPopupMenu.Create(self);
  FFiltersSubMenu := TfpgPopupMenu.Create(self);
  FHelpSubMenu  := TfpgPopupMenu.Create(self);

  // Populate File submenu
  with FFileSubMenu do
  begin
    AddMenuItem('&Novo', 'Ctrl+N', @DoNew);
    AddSeparator;
    AddMenuItem('&Abrir...', 'Ctrl+O', @DoOpen);
    AddMenuItem('&Guardar...', 'Ctrl+S', @DoSave);
    AddMenuItem('Exportar &CSV...', '', @DoExport);
    AddSeparator;
    AddMenuItem('Sai&r', 'Alt+F4', @DoExit);
  end;

  // Populate Tasks submenu
  with FTasksSubMenu do
  begin
    AddMenuItem('&Inserir...', 'Ins', @DoAddTask);
    AddMenuItem('&Editar...', 'Home', @DoEditTask);
    AddMenuItem('Alternar &Imediata', 'PgUp', @DoToggleTop);
    AddMenuItem('Alternar &Concluída', 'PgDn', @DoToggleDone);
    AddMenuItem('Editar &Nota', 'Space', @DoNoteTask);
    AddSeparator;
    AddMenuItem('&Apagar...', 'Del', @DoDeleteTask);
  end;
  
  // Filtros
  with FFiltersSubMenu do
  begin
    AddMenuItem('&Imediatas',  'Ctrl+0', @DoFilterTop);
    AddMenuItem('&Prioridade',  'Ctrl+1', @DoFilterByPriority);
    AddMenuItem('&Data de Fim', 'Ctrl+2', @DoFilterByDate);
    AddMenuItem('E&m curso',    'Ctrl+3', @DoFilterInProgress);
    AddMenuItem('&Concluídas',  'Ctrl+4', @DoFilterDone);
    AddMenuItem('&Todas',        'Ctrl+5', @DoFilterAll);
  end;

  // Populate Help submenu
  with FHelpSubMenu do
  begin
    AddMenuItem('&Sobre...', 'F1', @DoAbout);
  end;

  // Attach to menu bar
  Menu.AddMenuItem('&Ficheiro', nil).SubMenu := FFileSubMenu;
  Menu.AddMenuItem('&Tarefas',  nil).SubMenu := FTasksSubMenu;
  Menu.AddMenuItem('&Filtros', nil).SubMenu := FFiltersSubMenu;
  Menu.AddMenuItem('&Ajuda',    nil).SubMenu := FHelpSubMenu;
end;
