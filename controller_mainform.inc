{
               FPGenda - O código da tua produtividade.
               
                        versão 0.5 / 20251103
                        
    Copyright (c) 2025 by Duarte Mendes.
    
          >>> INCLUDE FILE : controller_mainform.inc <<<
    
    Part of FPGenda software build.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Descrição:
      CONTROLLER DO MAINFORM
}


(* All Helpers and Controllers for TMainForm *)


//HELPERS
procedure TMainForm.RefreshGrid;
var
  i: Integer; t: TTask;
begin
  Grid.RowCount := 0;
  for i := 0 to Tasks.Count-1 do
  begin
    t := Tasks[i];
    Grid.RowCount := Grid.RowCount + 1;
    Grid.Cells[0, Grid.RowCount-1] := IntToStr(t.ID);
    Grid.Cells[1, Grid.RowCount-1] := IntToStr(t.Prioridade);
    
    if Length(t.Inicio) = 8 then
      Grid.Cells[2, Grid.RowCount-1] := Copy(t.Inicio,1,4)+'-'+Copy(t.Inicio,5,2)+'-'+Copy(t.Inicio,7,2)
    else
      Grid.Cells[2, Grid.RowCount-1] := t.Inicio;
      
    if Length(t.Fim) = 8 then
      Grid.Cells[3, Grid.RowCount-1] := Copy(t.Fim,1,4)+'-'+Copy(t.Fim,5,2)+'-'+Copy(t.Fim,7,2)
    else
      Grid.Cells[3, Grid.RowCount-1] := t.Fim;
    
    Grid.Cells[4, Grid.RowCount-1] := t.Nome;
    
    Grid.Cells[5, Grid.RowCount-1] := t.Projecto;

    if t.PrioMax then
      Grid.Cells[6, Grid.RowCount-1] := '[X]'
    else
      Grid.Cells[6, Grid.RowCount-1] := '[ ]';
      
    if t.Concluida then
      Grid.Cells[7, Grid.RowCount-1] := '[X]'
    else
      Grid.Cells[7, Grid.RowCount-1] := '[ ]';
      
    if Length(t.Notas) > 0 then
      Grid.Cells[8, Grid.RowCount-1] := '[X]'
    else
      Grid.Cells[8, Grid.RowCount-1] := '[ ]';

  end;
end;

function TMainForm.SafeDateKey(const S: string): string;
var d: string; i: integer;
begin
  // mantém só dígitos e normaliza para AAAAMMDD; vazios/curtos vão para '00000000'
  d := '';
  for i := 1 to Length(S) do
    if S[i] in ['0'..'9'] then d := d + S[i];
  if Length(d) = 8 then
    Exit(d)
  else
    Exit('00000000');
end;

procedure TMainForm.RefreshGridFromIndex(const Idx: array of Integer);
var k, r: Integer; t: TTask;
begin
  Grid.RowCount := 0;
  for k := 0 to High(Idx) do
  begin
    t := Tasks[Idx[k]];
    Grid.RowCount := Grid.RowCount + 1;
    r := Grid.RowCount - 1;
    Grid.Cells[0, r] := IntToStr(t.ID);
    Grid.Cells[1, r] := IntToStr(t.Prioridade);
    
    if Length(t.Inicio) = 8 then
      Grid.Cells[2, r] := Copy(t.Inicio,1,4)+'-'+Copy(t.Inicio,5,2)+'-'+Copy(t.Inicio,7,2)
    else
      Grid.Cells[2, r] := t.Inicio;
    
    if Length(t.Fim) = 8 then
      Grid.Cells[3, r] := Copy(t.Fim,1,4)+'-'+Copy(t.Fim,5,2)+'-'+Copy(t.Fim,7,2)
    else
      Grid.Cells[3, r] := t.Fim;
     
     Grid.Cells[4, r] := t.Nome;
     Grid.Cells[5, r] := t.Projecto;
     
    if (t.PrioMax) then
      Grid.Cells[6, r] := '[X]'
    else
      Grid.Cells[6, r] := '[ ]';
      
      if (t.Concluida) then
        Grid.Cells[7, r] := '[X]'
      else
        Grid.Cells[7, r] := '[ ]';
        
              if Length(t.Notas) > 0 then
        Grid.Cells[8, r] := '[X]'
      else
        Grid.Cells[8, r] := '[ ]';
        
  end;
  
  if Grid.RowCount > 0 then
    Grid.FocusRow := 0
  else
    Grid.FocusRow := -1;
end;

//Criação da base de dados e conexão
procedure TMainForm.EnsureSQLiteDB(const AFileName: string; out AConn: TSQLite3Connection; out ATrans: TSQLTransaction);
begin
  AConn := TSQLite3Connection.Create(nil);
  ATrans := TSQLTransaction.Create(nil);
  AConn.DatabaseName := AFileName;
  AConn.Transaction := ATrans;
  AConn.Open;

  // criar tabela se não existir
  AConn.ExecuteDirect(
    'CREATE TABLE IF NOT EXISTS tasks ('+
    ' id INTEGER PRIMARY KEY,'+
    ' pri INTEGER,'+
    ' inicio TEXT,'+
    ' fim TEXT,'+
    ' nome TEXT,'+
    ' projeto TEXT,'+
    ' toprio INTEGER,'+
    ' done INTEGER,' +
    ' notas TEXT)');
  ATrans.Commit;
end;


// FILE CONTROLLERS
procedure TMainForm.DoNew(Sender: TObject);

begin

  if (Tasks.Count > 0) and
     (AskYesNo('Confirmar', 'Limpar a lista atual?') <> mbYes) then Exit;

    Tasks.Clear;
    RefreshGrid;
    
end;

// --------------- OPEN FILE
procedure TMainForm.DoOpen(Sender: TObject);
var
  fn: string;
  conn: TSQLite3Connection;
  trans: TSQLTransaction;
  q: TSQLQuery;
  t: TTask;
  dlg: TfpgFileDialog;
  maxID: Integer;
begin
    
  dlg := TfpgFileDialog.Create(nil);
  try
    dlg.Filename := 'agenda.db';
    dlg.Filter := 'SQLite DB (*.db,*.sqlite)|*.db; *.sqlite|Todos (*.*)|*.*';
    if dlg.RunOpenFile then //Botão OK
    begin
      fn := dlg.FileName;

      EnsureSQLiteDB(fn, conn, trans);
      try
        q := TSQLQuery.Create(nil);
        try
          q.Database := conn;
          q.Transaction := trans;
          q.SQL.Text := 'SELECT id, pri, inicio, fim, nome, projeto, toprio, done, notas FROM tasks ORDER BY id';
          q.Open;

          // limpar lista atual
          Tasks.Clear;
          maxID := 0;

          while not q.EOF do
          begin
            t := TTask.Create;
            t.ID         := q.FieldByName('id').AsInteger;
            t.Prioridade := q.FieldByName('pri').AsInteger;
            t.Inicio     := q.FieldByName('inicio').AsString;
            t.Fim        := q.FieldByName('fim').AsString;
            t.Nome       := q.FieldByName('nome').AsString;
            t.Projecto   := q.FieldByName('projeto').AsString;
            t.PrioMax    := q.FieldByName('toprio').AsInteger <> 0;
            t.Concluida  := q.FieldByName('done').AsInteger <> 0;
            t.Notas      := q.FieldByName('notas').AsString;

            Tasks.AddTaskObj(t);  // <- vê nota abaixo
            if t.ID > maxID then maxID := t.ID;

            q.Next;
          end;
          // se a tua lista gera IDs automaticamente, atualiza aqui
          Tasks.NextID;
          RefreshGrid;
          ShowMessage('Agenda carregada de SQLite.','Info');
        finally
          q.Free;
        end;
      finally
        conn.Free;
        trans.Free;
      end;
      
    end; // Begin do RunOpenFile...
        //Botão Cancel...
  finally
    dlg.Free;
  end;
end;

// --------------- SAVE FILE
procedure TMainForm.DoSave(Sender: TObject);
var
  fn: string;
  conn: TSQLite3Connection;
  trans: TSQLTransaction;
  q: TSQLQuery;
  i: Integer;
  t: TTask;
  dlg: TfpgFileDialog;
  
begin

  dlg := TfpgFileDialog.Create(nil);
  try
    dlg.Filename := 'agenda.db';
    dlg.Filter := 'SQLite DB (*.db,*.sqlite)|*.db; *.sqlite|Todos (*.*)|*.*';
    if dlg.RunSaveFile then //Botão OK
    begin
      fn := dlg.FileName;

      EnsureSQLiteDB(fn, conn, trans);
      try
        // apaga conteúdo atual
        conn.ExecuteDirect('DELETE FROM tasks');
        // prepara insert
        q := TSQLQuery.Create(nil);
        try
          q.Database := conn;
          q.Transaction := trans;
          q.SQL.Text :=
            'INSERT INTO tasks (id, pri, inicio, fim, nome, projeto, toprio, done, notas) '+
            'VALUES (:id, :pri, :inicio, :fim, :nome, :projeto, :toprio, :done, :notas)';
          for i := 0 to Tasks.Count-1 do
          begin
            t := Tasks[i];
            q.ParamByName('id').AsInteger      := t.ID;
            q.ParamByName('pri').AsInteger     := t.Prioridade;
            q.ParamByName('inicio').AsString   := t.Inicio;
            q.ParamByName('fim').AsString      := t.Fim;
            q.ParamByName('nome').AsString     := t.Nome;
            q.ParamByName('projeto').AsString  := t.Projecto;
            q.ParamByName('toprio').AsInteger  := Ord(t.PrioMax);
            q.ParamByName('done').AsInteger    := Ord(t.Concluida);
            q.ParamByName('notas').AsString    := t.Notas;
            q.ExecSQL;
          end;
          trans.Commit;
          ShowMessage('Agenda guardada em SQLite.','Info');
        finally
          q.Free;
        end;
      finally
        conn.Free;
        trans.Free;
      end;
      
    end; // Begin do RunSaveFile...
        //Botão Cancel...
  finally
    dlg.Free;
  end;    
      
end;

// --------------- EXPORT CSV FILE
procedure TMainForm.DoExport(Sender: TObject);
var dlg: TfpgFileDialog; fn: string;
begin
  dlg := TfpgFileDialog.Create(self);
  try
    dlg.Filter := 'CSV|*.csv|Todos|*.*';
    dlg.FileName := 'tarefas.csv';
    if dlg.RunSaveFile then
    begin
      fn := dlg.FileName;
      Tasks.ExportCSV(fn);
      ShowMessage('Exportado para: '+fn, 'Exportar CSV');
    end;
  finally
    dlg.Free;
  end;
end;


procedure TMainForm.DoExit(Sender: TObject);
begin
  Close;
end;


// TASK CONTROLLERS
procedure TMainForm.DoAddTask(Sender: TObject);
var
  pri: Integer; Inicio, Fim, nome, proj, notas: string;
  toprio, done: Boolean;
  dlg: TTaskDialog;
  
begin
  // valores por defeito
  pri := 5; Inicio := ''; Fim := ''; nome := ''; proj := '';
  toprio := False; done := False; notas :='';
  
  while True do
  begin
    dlg := TTaskDialog.Create(Self);// Estamos a criar SEMPRE em WINDOWS (LINUX NÃO SERIA NECESSÁRIO) 
    try
      // passa os valores da tentativa anterior
      if dlg.Execute(pri, Inicio, Fim, nome, proj, toprio, done) then
      begin
        // OK e válido
        Tasks.Add(pri, Inicio, Fim, nome, proj, toprio, done, notas);
        RefreshGrid;
        Break;
      end
      else
      begin
        // se não foi erro de validação → utilizador cancelou → sai
        if not dlg.ValidationFailed then
          Break;
        // se foi erro de validação → volta ao while, mas com NOVO dlg
        // e com os campos já preenchidos (porque atualizámos as vars)
      end;
    finally
      dlg.Free;
    end;
  end;
end;


procedure TMainForm.DoEditTask(Sender: TObject);
var
  sel, id, idx: Integer;
  t: TTask;
  pri: Integer; Inicio, Fim, nome, proj: string;
  toprio, done: Boolean;
  dlg: TTaskDialog;
  
begin
  sel := Grid.FocusRow;
  if (sel < 0) or (sel >= Tasks.Count) then Exit;

  id  := StrToIntDef(Grid.Cells[0, sel], -1);
  idx := Tasks.FindByID(id);
  if idx < 0 then Exit;

  t       := Tasks[idx];
  pri     := t.Prioridade;
  Inicio  := t.Inicio;
  Fim     := t.Fim;
  nome    := t.Nome;
  proj    := t.Projecto;
  toprio  := t.PrioMax;
  done    := t.Concluida;
  
  while True do
  begin
    dlg := TTaskDialog.Create(Self); // Estamos a criar SEMPRE em WINDOWS (LINUX NÃO SERIA NECESSÁRIO) 
    // um novo Dialog porque quando o Showmodal termina ele desanexa os widgets da janela ...
    try
      if dlg.Execute(pri, Inicio, Fim, nome, proj, toprio, done) then
      begin
        t.Prioridade := pri;
        t.Inicio     := Inicio;
        t.Fim        := Fim;
        t.Nome       := nome;
        t.Projecto   := proj;
        t.PrioMax    := toprio;
        t.Concluida  := done;
        RefreshGrid;
        Grid.FocusRow := sel;
        Break;
      end
      else
      begin
        if not dlg.ValidationFailed then
          Break;
      end;
    finally
      dlg.Free;
    end;
  end;
end;


procedure TMainForm.DoDeleteTask(Sender: TObject);
var sel, id, idx: Integer;
begin
  sel := Grid.FocusRow;
  if (sel < 0) or (sel >= Tasks.Count) then Exit;

  if AskYesNo('Confirmar', 'Apagar tarefa selecionada?') <> mbYes then Exit;

  id := StrToIntDef(Grid.Cells[0, sel], -1);
  idx := Tasks.FindByID(id);
  if idx >= 0 then
  begin
    Tasks.DeleteByIndex(idx);
    RefreshGrid;
  end;
end;


procedure TMainForm.DoNoteTask(Sender: TObject);
var
  sel, id, idx: Integer;
  t: TTask;
  dlg: TNoteDialog;

begin

  sel := Grid.FocusRow;
  if (sel < 0) or (sel >= Tasks.Count) then Exit;

  id  := StrToIntDef(Grid.Cells[0, sel], -1);
  idx := Tasks.FindByID(id);
  if idx < 0 then Exit;

  t       := Tasks[idx];

  ActiveTaskName := t.Nome; //Name of task as global va to pass to label
  dlg := TNoteDialog.Create(Self);
  dlg.note.Text := t.notas; // Actualiza o texto do memo com as notas

  try
      if dlg.ShowModal = TfpgModalResult(1) then
      begin
        t.Notas       := Trim(dlg.note.Text);
        RefreshGrid;
        Grid.FocusRow := sel;
      End;
   finally  
      dlg.Free;
   end;

end;


procedure TMainForm.DoToggleTop(Sender: TObject);
var sel, id, idx: Integer;
begin
  sel := Grid.FocusRow;
  if (sel < 0) or (sel >= Tasks.Count) then Exit;

  id := StrToIntDef(Grid.Cells[0, sel], -1);
  idx := Tasks.FindByID(id);
  if idx >= 0 then
  begin
    Tasks.ToggleTopByIndex(idx);
    RefreshGrid;
  end;
end;


procedure TMainForm.DoToggleDone(Sender: TObject);
var sel, id, idx: Integer;
begin
  sel := Grid.FocusRow;
  if (sel < 0) or (sel >= Tasks.Count) then Exit;

  id := StrToIntDef(Grid.Cells[0, sel], -1);
  idx := Tasks.FindByID(id);
  if idx >= 0 then
  begin
    Tasks.ToggleDoneByIndex(idx);
    RefreshGrid;
  end;
end;


// FILTERS CONTROLLERS
procedure TMainForm.DoFilterAll(Sender: TObject);
begin
  RefreshGrid; // já tens este método a listar tudo na ordem natural
end;

procedure TMainForm.DoFilterByPriority(Sender: TObject);
var idx: array of Integer;
    i, j, tmp: Integer;
begin
  // junta todos
  SetLength(idx, Tasks.Count);
  for i := 0 to Tasks.Count-1 do idx[i] := i;

  // ordena por prioridade desc (5..1) — simples bubble/selection sort
  for i := 0 to High(idx)-1 do
    for j := i+1 to High(idx) do
      if Tasks[idx[i]].Prioridade < Tasks[idx[j]].Prioridade then
      begin
        tmp := idx[i]; idx[i] := idx[j]; idx[j] := tmp;
      end;

  RefreshGridFromIndex(idx);
end;

procedure TMainForm.DoFilterByDate(Sender: TObject);
var idx: array of Integer;
    i, j, tmp: Integer;
    a, b: string;
begin
  // junta todos
  SetLength(idx, Tasks.Count);
  for i := 0 to Tasks.Count-1 do idx[i] := i;

  // ordena por Fim asc (mais antigo primeiro) usando chave AAAAMMDD
  for i := 0 to High(idx)-1 do
    for j := i+1 to High(idx) do
    begin
      a := SafeDateKey(Tasks[idx[i]].Fim);
      b := SafeDateKey(Tasks[idx[j]].Fim);
      if a > b then
      begin
        tmp := idx[i]; idx[i] := idx[j]; idx[j] := tmp;
      end;
    end;

  RefreshGridFromIndex(idx);
end;

procedure TMainForm.DoFilterInProgress(Sender: TObject);
var idx: array of Integer;
    i, n: Integer;
begin
  // só não concluídas, ordem natural
  SetLength(idx, Tasks.Count);
  n := 0;
  for i := 0 to Tasks.Count-1 do
    if not Tasks[i].Concluida then
    begin
      idx[n] := i; Inc(n);
    end;
  SetLength(idx, n);
  RefreshGridFromIndex(idx);
end;

procedure TMainForm.DoFilterDone(Sender: TObject);
var idx: array of Integer;
    i, n: Integer;
begin
  // só concluídas, ordem natural
  SetLength(idx, Tasks.Count);
  n := 0;
  for i := 0 to Tasks.Count-1 do
    if Tasks[i].Concluida then
    begin
      idx[n] := i; Inc(n);
    end;
  SetLength(idx, n);
  RefreshGridFromIndex(idx);
end;


procedure TMainForm.DoFilterTop(Sender: TObject);
var idx: array of Integer;
    i, n: Integer;
begin
  // só tarefas TOP - PrioMax
  SetLength(idx, Tasks.Count);
  n := 0;
  for i := 0 to Tasks.Count-1 do
    if Tasks[i].PrioMax then
    begin
      idx[n] := i; Inc(n);
    end;
  SetLength(idx, n);
  RefreshGridFromIndex(idx);
end;


//ABOUT / INFO
procedure TMainForm.DoAbout(Sender: TObject);
begin
  ShowMessage(
    '+++++ { FPGenda - v0.5 } +++++'+LineEnding+
    'O código da tua produtividade.'+LineEnding+
    ''+LineEnding+
    ' by Duarte Mendes , 20251103'+LineEnding+
    ''+LineEnding+
    'Ferramentas:'+LineEnding+
    '    Free Pascal'+LineEnding+
    '    SQLite'+LineEnding+
    '    fpGUI © Graeme Geldenhuys.'+LineEnding+
    'Optimizado com ChatGPT 5.','Sobre/Info');
end;
