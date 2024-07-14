import { getIsGenerating, getIsSolving } from './mazeLogic.js';
import { showToast, updateCellClass, closeMenu } from './userInterface.js';
import { mazeContainer } from './controller.js';

let isMouseDown = false;
let lastCellId = '';
let isDragging = false;

function moveEntryExit() {
	let draggedCell = null;
	let originalCellId = '';

	// Identify the cell to be moved
	mazeContainer.addEventListener('mousedown', (event) => {
		if (event.target && event.target.tagName === 'BUTTON' && event.button === 0) {
			const cell = event.target;
			const cellId = cell.id;
			const cellClasses = cell.classList;

			if (cellClasses.contains('entry') || cellClasses.contains('exit')) {
				if (getIsGenerating() || getIsSolving()) {
					showToast('warning', 'Warning', 'Maze is currently being generated or solved! Please wait.');
					return;
				}

				draggedCell = cell;
				originalCellId = cellId;
				draggedCell.classList.add('dragging');
				isDragging = true;
			}
		}
	});

	// Move entry/exit to the new empty cell
	mazeContainer.addEventListener('mousemove', (event) => {
		if (draggedCell) {
			const [_, origY, origX] = originalCellId.split('-').map(Number);
			const target = event.target;

			if (target && !target.classList.contains('wall')) {
				if (
					(draggedCell.classList.contains('entry') && !target.classList.contains('exit')) ||
					(draggedCell.classList.contains('exit') && !target.classList.contains('entry'))
				) {
					target.classList.add('dragging');
				}
			}

			if (target && target.tagName === 'BUTTON' && target !== draggedCell) {
				const [_, targetY, targetX] = target.id.split('-').map(Number);
				const cellType = Module.ccall('getTypeCell', 'number', ['number', 'number'], [targetY, targetX]);

				if (cellType === 0 || cellType === 2147483646) {
					let entryExit = -2;

					// Add the class to the new cell
					if (draggedCell.classList.contains('entry')) {
						target.classList.add('entry');
						entryExit = 3;
					} else if (draggedCell.classList.contains('exit')) {
						target.classList.add('exit');
						entryExit = 4;
					}

					// Remove the class from the original cell
					draggedCell.classList.remove('entry', 'exit');

					// Update the cell state
					Module.ccall('setTypeCell', null, ['number', 'number', 'number'], [targetY, targetX, entryExit]);
					Module.ccall('setTypeCell', null, ['number', 'number', 'number'], [origY, origX, 0]);

					// Update the UI
					updateCellClass(targetY, targetX, 0);
					updateCellClass(origY, origX, 0);

					// Update original cell ID
					originalCellId = target.id;
					draggedCell = target;
				}
			}
		}
	});

	// Reset variables and remove .dragging class
	mazeContainer.addEventListener('mouseup', () => {
		if (draggedCell) {
			draggedCell.classList.remove('dragging');
			draggedCell = null;
			isDragging = false;
		}
	});

	mazeContainer.addEventListener('mouseleave', () => {
		if (draggedCell) {
			draggedCell.classList.remove('dragging');
			draggedCell = null;
			isDragging = false;
		}
	});

	// For touch mode
	// Identify the cell to be moved
	mazeContainer.addEventListener('touchstart', (event) => {
		closeMenu();
		const touch = event.touches[0];
		const target = document.elementFromPoint(touch.clientX, touch.clientY);

		if (target && target.tagName === 'BUTTON') {
			const cell = target;
			const cellId = cell.id;
			const cellClasses = cell.classList;

			if (cellClasses.contains('entry') || cellClasses.contains('exit')) {
				if (getIsGenerating() || getIsSolving()) {
					showToast('warning', 'Warning', 'Maze is currently being generated or solved! Please wait.');
					return;
				}

				draggedCell = cell;
				originalCellId = cellId;
				draggedCell.classList.add('dragging');
				isDragging = true;
			}
		}
	});

	// Move entry/exit to the new empty cell
	mazeContainer.addEventListener('touchmove', (event) => {
		if (draggedCell) {
			const [_, origY, origX] = originalCellId.split('-').map(Number);
			const touch = event.touches[0];
			const target = document.elementFromPoint(touch.clientX, touch.clientY);

			if (target && !target.classList.contains('wall')) {
				if (
					(draggedCell.classList.contains('entry') && !target.classList.contains('exit')) ||
					(draggedCell.classList.contains('exit') && !target.classList.contains('entry'))
				) {
					target.classList.add('dragging');
				}
			}

			if (target && target.tagName === 'BUTTON' && target !== draggedCell) {
				const [_, targetY, targetX] = target.id.split('-').map(Number);
				const cellType = Module.ccall('getTypeCell', 'number', ['number', 'number'], [targetY, targetX]);

				if (cellType === 0 || cellType === 2147483646) {
					let entryExit = -2;

					// Add the class to the new cell
					if (draggedCell.classList.contains('entry')) {
						target.classList.add('entry');
						entryExit = 3;
					} else if (draggedCell.classList.contains('exit')) {
						target.classList.add('exit');
						entryExit = 4;
					}

					// Remove the class from the original cell
					draggedCell.classList.remove('entry', 'exit');

					// Update the cell state
					Module.ccall('setTypeCell', null, ['number', 'number', 'number'], [targetY, targetX, entryExit]);
					Module.ccall('setTypeCell', null, ['number', 'number', 'number'], [origY, origX, 0]);

					// Update the UI
					updateCellClass(targetY, targetX, 0);
					updateCellClass(origY, origX, 0);

					// Update original cell ID
					originalCellId = target.id;
					draggedCell = target;
				}
			}
		}
	});

	// Reset variables and remove .dragging class
	mazeContainer.addEventListener('touchend', () => {
		if (draggedCell) {
			draggedCell.classList.remove('dragging');
			draggedCell = null;
			isDragging = false;
		}
	});
}

function toggleCellType(y, x, generatorSelect) {
	closeMenu();
	const cell = document.getElementById(`cell-${y}-${x}`);
	let cellType = Module.ccall('getTypeCell', 'number', ['number', 'number'], [y, x]);

	if (getIsGenerating() || getIsSolving()) {
		showToast('warning', 'Warning', 'Maze is currently being generated or solved! Please wait.');
		return;
	}

	if (generatorSelect.selectedIndex !== 6) {
		generatorSelect.selectedIndex = 6;
		document.querySelector('#generateButton').disabled = true;
		showToast('info', 'Info', 'You are now in custom mode! Feel free to modify the maze.');
	}

	if (cell && cellType !== 3 && cellType !== 4) {
		cell.className = '';

		if (cellType === -1) {
			cell.classList.add('cell');
			cellType = 0;
		} else {
			cell.classList.add('wallAnimation', 'cell', 'wall');
			cellType = -1;
		}
	}

	Module.ccall('setTypeCell', null, ['number', 'number', 'number'], [y, x, cellType]);
}

export function setMouseEvents(mazeContainer, generatorSelect) {
	moveEntryExit();

	mazeContainer.addEventListener('mousedown', (event) => {
		if (event.target && event.target.tagName === 'BUTTON' && event.button === 0) {
			const cellId = event.target.id;
			const [_, y, x] = cellId.split('-').map(Number);
			isMouseDown = true;
			lastCellId = cellId;

			if (!isDragging) {
				toggleCellType(y, x, generatorSelect);
			}
		}
	});

	mazeContainer.addEventListener('mousemove', (event) => {
		if (isMouseDown && event.target && event.target.tagName === 'BUTTON') {
			const cellId = event.target.id;

			if (cellId !== lastCellId) {
				lastCellId = cellId;
				const [_, y, x] = cellId.split('-').map(Number);

				if (!isDragging) {
					toggleCellType(y, x, generatorSelect);
				}
			}
		}
	});

	mazeContainer.addEventListener('mouseup', () => {
		isMouseDown = false;
	});

	mazeContainer.addEventListener('mouseleave', () => {
		isMouseDown = false;
	});

	// For touch mode
	mazeContainer.addEventListener('touchstart', (event) => {
		isMouseDown = true;
	});

	mazeContainer.addEventListener('touchmove', (event) => {
		const touch = event.touches[0];
		const target = document.elementFromPoint(touch.clientX, touch.clientY);

		if (isMouseDown && target && target.tagName === 'BUTTON') {
			const cellId = target.id;

			if (cellId !== lastCellId) {
				lastCellId = cellId;
				const [_, y, x] = cellId.split('-').map(Number);

				if (!isDragging) {
					toggleCellType(y, x, generatorSelect);
				}
			}
		}
	});

	mazeContainer.addEventListener('touchend', () => {
		isMouseDown = false;
	});
}
