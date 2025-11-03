{
               FPGenda - O código da tua produtividade.
               
                        versão 0.5 / 20251103
                        
    Copyright (c) 2025 by Duarte Mendes.
    
                  >>> UNIT : myfpg_utils.pas <<<
    
    Part of FPGenda software build.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Descrição:
      Esta unit contém utilitários para uso genérico.
      Compile with fpGUI >= 1.5 and FreePascal >= 3.2.2 
}


unit myfpg_utils;

interface

uses
  SysUtils, 
  fpg_main, fpg_button, fpg_dialogs, fpg_base;


//Returns the string of a Question in a message dialog with OK/Cancel buttons
function AskQuestion(const ATitle, AQuestion: string): TfpgString;

//Returns the Button pressed from a Question dialog
function AskYesNo(const ATitle, AQuestion: string): TfpgMsgDlgBtn;

//Resturns a string with digits; if len > 0 then have digits
function OnlyDigits(const S: String): String;

//Validates the date format YYYYMMDD from a string
function IsYYYYMMDD(const S: String): Boolean;

//Calculates the horizontal (x)central position for a Text between 2 coordinates in pixels
// It assumes 6 pixels for character
function CenterText(const s: String; const xi, xf: word): LongInt;

{ === Utils === }
implementation

//Returns the STRING of a Question in a message dialog OK/Cancel
(*usage:

var resultado : TfpgString;
...
resultado := AskQuestion('Confirmar', 'Limpar a lista atual?');
*)

function AskQuestion(const ATitle, AQuestion: string): TfpgString;
var s: TfpgString;

begin
  s := '';
  while (s='') do
  begin
    if fpgInputQuery(ATitle, AQuestion, s) then
       s:=Trim(s)
    else
       break;
  end;
    Result := s ;
end;


//Returns the Button pressed from a Question dialog
(*usage:
If AskYesNo('Confirmar', 'Limpar a lista atual?') = mbYes then
  ...
Else
  ...
*)
function AskYesNo(const ATitle, AQuestion: string): TfpgMsgDlgBtn;
var dlg: TfpgMessageDialog;

begin

    dlg := TfpgMessageDialog.Create(nil);
  try
    dlg.Buttons := [mbYes, mbNo];   // ou [mbOK, mbCancel]
    Result:= dlg.Question(ATitle, AQuestion, dlg.Buttons, mbYes, mbNo)
  finally
    dlg.Free;
  end;
end;


function OnlyDigits(const S: String): String;
var i: Integer;
begin
  Result := '';
  for i := 1 to Length(S) do
    if S[i] in ['0'..'9'] then Result := Result + S[i];
end;


function IsYYYYMMDD(const S: String): Boolean;
var y, m, d: Integer;
begin
  Result := False;
  if Length(S) <> 8 then Exit;
  y := StrToIntDef(Copy(S,1,4), -1);
  m := StrToIntDef(Copy(S,5,2), -1);
  d := StrToIntDef(Copy(S,7,2), -1);
  try
    if (y>0) and (m in [1..12]) and (d>=1) then
    begin
      // valida data pelo EncodeDate
      EncodeDate(y, m, d);
      Result := True;
    end;
  except
    Result := False;
  end;
end;


(*usage:
    <object>.Left := CenterText('Texto a centrar', 10, 640);
*)
function CenterText(const s: String; const xi, xf: Word): LongInt;

begin
  Result := Round(((xf - Length(s) * 6) + xi) / 2); //6 pixels for each character
end;

END.
