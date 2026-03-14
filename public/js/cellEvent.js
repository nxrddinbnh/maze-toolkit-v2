import { CellType } from './constants.js';
import { getIsGenerating, getIsSolving } from './mazeLogic.js';
import { showToast, updateCellClass, closeMenu } from './userInterface.js';
import { mazeContainer } from './controller.js';

let isMouseDown = false;
let lastCellId = '';
let isDragging = false;

function parseCellId(id) {
	const [, y, x] = id.split('-').map(Number);
	return { y, x };
}

function guardAgainstAnimation() {
	if (getIsGenerating() || getIsSolving()) {
		showToast('warning', 'Warning', 'Maze is currently being generated or solved! Please wait.');
		return true;
	}
	return false;
}

function moveEntryExit() {
	let draggedCell = null;
	let originalCellId = '';

	function startDrag(cell) {
		if (guardAgainstAnimation()) return;
		draggedCell = cell;
		originalCellId = cell.id;
		draggedCell.classList.add('dragging');
		isDragging = true;
	}

	function tryMoveDraggedCell(target) {
		if (!draggedCell || !target || target.tagName !== 'BUTTON' || target === draggedCell) return;

		const { y: origY, x: origX } = parseCellId(originalCellId);
		const { y: targetY, x: targetX } = parseCellId(target.id);
		const cellType = Module.ccall('getTypeCell', 'number', ['number', 'number'], [targetY, targetX]);

		if (cellType !== CellType.EMPTY && cellType !== CellType.PATH) return;

		let entryExitType;
		if (draggedCell.classList.contains('entry')) {
			target.classList.add('entry');
			entryExitType = CellType.ENTRY;
		} else if (draggedCell.classList.contains('exit')) {
			target.classList.add('exit');
			entryExitType = CellType.EXIT;
		} else {
			return;
		}

		draggedCell.classList.remove('entry', 'exit');

		Module.ccall('setTypeCell', null, ['number', 'number', 'number'], [targetY, targetX, entryExitType]);
		Module.ccall('setTypeCell', null, ['number', 'number', 'number'], [origY, origX, CellType.EMPTY]);

		updateCellClass(targetY, targetX, 0);
		updateCellClass(origY, origX, 0);

		originalCellId = target.id;
		draggedCell = target;
	}

	function highlightDragTarget(target) {
		if (!target || target.classList.contains('wall')) return;

		const isEntry = draggedCell?.classList.contains('entry');
		const isExit  = draggedCell?.classList.contains('exit');

		if ((isEntry && !target.classList.contains('exit')) ||
		    (isExit  && !target.classList.contains('entry'))) {
			target.classList.add('dragging');
		}
	}

	function endDrag() {
		if (draggedCell) {
			draggedCell.classList.remove('dragging');
			draggedCell = null;
			isDragging = false;
		}
	}

	// Mouse events
	mazeContainer.addEventListener('mousedown', (event) => {
		if (event.button !== 0 || event.target?.tagName !== 'BUTTON') return;

		const cell = event.target;
		if (cell.classList.contains('entry') || cell.classList.contains('exit')) {
			startDrag(cell);
		}
	});

	mazeContainer.addEventListener('mousemove', (event) => {
		if (!draggedCell) return;
		highlightDragTarget(event.target);
		tryMoveDraggedCell(event.target);
	});

	mazeContainer.addEventListener('mouseup', endDrag);
	mazeContainer.addEventListener('mouseleave', endDrag);

	// Touch events
	mazeContainer.addEventListener('touchstart', (event) => {
		closeMenu();
		const target = document.elementFromPoint(event.touches[0].clientX, event.touches[0].clientY);
		if (target?.tagName === 'BUTTON' &&
		    (target.classList.contains('entry') || target.classList.contains('exit'))) {
			startDrag(target);
		}
	});

	mazeContainer.addEventListener('touchmove', (event) => {
		if (!draggedCell) return;
		const target = document.elementFromPoint(event.touches[0].clientX, event.touches[0].clientY);
		highlightDragTarget(target);
		tryMoveDraggedCell(target);
	});

	mazeContainer.addEventListener('touchend', endDrag);
}

function toggleCellType(y, x, generatorSelect) {
	closeMenu();

	if (guardAgainstAnimation()) return;

	if (generatorSelect.selectedIndex !== 6) {
		generatorSelect.selectedIndex = 6;
		document.querySelector('#generateButton').disabled = true;
		showToast('info', 'Info', 'You are now in custom mode! Feel free to modify the maze.');
	}

	const cell = document.getElementById(`cell-${y}-${x}`);
	const cellType = Module.ccall('getTypeCell', 'number', ['number', 'number'], [y, x]);

	if (!cell || cellType === CellType.ENTRY || cellType === CellType.EXIT) return;

	let newType;
	if (cellType === CellType.WALL) {
		cell.className = 'cell';
		newType = CellType.EMPTY;
	} else {
		cell.className = 'wallAnimation cell wall';
		newType = CellType.WALL;
	}

	Module.ccall('setTypeCell', null, ['number', 'number', 'number'], [y, x, newType]);
}

export function setMouseEvents(mazeContainer, generatorSelect) {
	moveEntryExit();

	mazeContainer.addEventListener('mousedown', (event) => {
		if (event.button !== 0 || event.target?.tagName !== 'BUTTON') return;

		const { y, x } = parseCellId(event.target.id);
		isMouseDown = true;
		lastCellId = event.target.id;

		if (!isDragging) {
			toggleCellType(y, x, generatorSelect);
		}
	});

	mazeContainer.addEventListener('mousemove', (event) => {
		if (!isMouseDown || event.target?.tagName !== 'BUTTON') return;

		const cellId = event.target.id;
		if (cellId === lastCellId) return;

		lastCellId = cellId;
		const { y, x } = parseCellId(cellId);

		if (!isDragging) {
			toggleCellType(y, x, generatorSelect);
		}
	});

	mazeContainer.addEventListener('mouseup',    () => { isMouseDown = false; });
	mazeContainer.addEventListener('mouseleave', () => { isMouseDown = false; });

	// Touch events
	mazeContainer.addEventListener('touchstart', () => { isMouseDown = true; });

	mazeContainer.addEventListener('touchmove', (event) => {
		const target = document.elementFromPoint(event.touches[0].clientX, event.touches[0].clientY);
		if (!isMouseDown || target?.tagName !== 'BUTTON') return;

		const cellId = target.id;
		if (cellId === lastCellId) return;

		lastCellId = cellId;
		const { y, x } = parseCellId(cellId);

		if (!isDragging) {
			toggleCellType(y, x, generatorSelect);
		}
	});

	mazeContainer.addEventListener('touchend', () => { isMouseDown = false; });
}
