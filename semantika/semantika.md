# **Sémantické kontroly**

## **Typy sémantické chyb a jejich error codes:**
	3 - sémantická chyba v programu - nedefinovaná funkce, pokus o redefinice funkce.
	4 - sémantická/běhová chyba v programu špatný počet/typ parametrů u volání funkce či typ návratové hodnoty z funkce.
	5 - sémantická chyba v programu - použití nedefinované proměnné.
	6 - sémantická/běhová chyba v programu - chybějící/přebývající výraz v příkazu návratu z funkce.
	7 - sémantická/běhová chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech.
	8 - ostatní sémantické chyby.

## **Chyba 3 - nedefinovaná funkce, pokus o redefinici funkce**
- **Každá funkce je definována právě 1**
	```
	function bar() : string {
		return "A";
	}

	function bar() : string { // chyba redefinice
		return "A";
	}

	function bar() : string { // opět chyba redefinice
		return "B";
	}
	```
- **Definice funkce musí lexikálně předcházet volání této funkce**
	- př. OK
	```
	function bar(string $param) : string {
		return foo($param);
	}

	function foo(string $param) : string {
		return bar($param);
	}

	bar("ahoj");
	```
	- Př. ERROR
	```
	bar("ahoj"); // chyba - funkce bar ještě nebyla definována

	function bar(string $param) : string {
		return $param;
	}
	```
	- Př. ERROR
	```
	function bar(string $param) : string {
		return foo($param);
	}

	bar("ahoj"); // chyba - funkce foo, kterou volá bar ještě nebyla definována

	function foo(string $param) : string {
		return bar($param);
	}
	```
- ### **Řešení**:
  - dostanu tokeny definice funkce X -> vyhledám funkci X v tabulce symbolů
	- **A)** symbol funkce X v tabulce již existuje
	  - **i)** symbol funkce X je označen jako nedefinovaný
		- změním info o symbolu a funkci X označím za definovanou
		- OK
	  - **ii)** symbol funkce X je označen jako definovaný
		- ERROR (redefinice funkce)
	- **B)** symbol v tabulce neexistuje
	  - přidám nový symbol funkce X do tabulky symbolů a označím X za definovanou
	  - OK
  - dostanu token volání funkce X -> vyhledám funkci X v tabulce symbolů
	- **A)** funkce X nemá v tabulce symbol
		- **i)** volání funkce X je unitř funkce Y
			1. vložím do globální tabulky nový symbol funkce X
			2. přidám informace o tom, že X není zatím definovaná
			3. do infa o symbolu nadřazené funkce Y přidám odkaz na symbol X 
		  - zatím OK 
		- **ii)** volání funkce X není uvnitř jiné funkce
		  - ERROR
	- **B)** funkce X má v tabulce symbol, ale je nedefinovaná
	  - ERROR
	- **C)** funkce X má v tabulce symbol a je definovaná
	  - podívám se do infa symbolu na odkazy na funkce, které jsou z této funkce volány
		- **i)** symbol funkce X nemá žádné odkazy na funkce, které volá
		  - OK
		- **ii)** symbol funkce X má odkazy na funkce, které jsou všechny definovány
		  - OK
		- **iii)** symbol funkce X má odkazy na funkce, které nebyly definovány
		  - ERROR

pozn. běhové chyby, které nelze zjistit při kompilaci, je nutné vyřešit, tak, že do vygenerováného kódu jsou přidány typové kontroly TYPE a v případě, že typ není správný, tak je program ukončen s určitým chybovým kódem