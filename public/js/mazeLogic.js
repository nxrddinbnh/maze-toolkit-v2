import { getGeneratorSpeedInterval, getSolverSpeedInterval, getGeneratorAlgo } from './controller.js';
import { showToast, updateCellClass, setControlsState, getWidth, getHeight } from './userInterface.js';

const state = {
	updateTimer: null,
	isGenerating: false,
	isSolving: false,
	hasPath: false,
};

export function getUpdateTimer() {
	return state.updateTimer;
}

export function setUpdateTimer(timer) {
	state.updateTimer = timer;
}

export function getIsGenerating() {
	return state.isGenerating;
}

export function setIsGenerating(newIsGenerating) {
	state.isGenerating = newIsGenerating;
}

export function getIsSolving() {
	return state.isSolving;
}

export function setIsSolving(newIsSolving) {
	state.isSolving = newIsSolving;
}

export function getHasPath() {
	return state.hasPath;
}

export function setHasPath(newHasPath) {
	state.hasPath = newHasPath;
}

export function prepareMaze() {
	let startType = getGeneratorAlgo();

	if (startType == 1 || startType == 3 || startType == 5 || startType == 6) {
		startType = 3;
	} else if (startType == 2) {
		startType = 2;
	} else {
		startType = 1;
	}

	Module.ccall('prepareMaze', null, ['number'], [startType]);

	for (let y = 1; y < getHeight() - 1; y++) {
		for (let x = 1; x < getWidth() - 1; x++) {
			updateCellClass(y, x, 0);
		}
	}
}

export function restorePath() {
	for (let y = 1; y < getHeight(); y++) {
		for (let x = 1; x < getWidth(); x++) {
			const cellType = Module.ccall('getTypeCell', 'number', ['number', 'number'], [y, x]);

			if (cellType === 2147483646) {
				Module.ccall('setTypeCell', null, ['number', 'number', 'number'], [y, x, 0]);
				updateCellClass(y, x, 0);
			}
		}
	}
}

function getCellOrder() {
	const cellOrderSize = Module.ccall('getCellOrderSize', 'number', [], []);
	let cellOrder = [];

	for (let i = 0; i < cellOrderSize; i++) {
		let ptrY = Module._malloc(4);
		let ptrX = Module._malloc(4);

		Module.ccall('getCellOrder', null, ['number', 'number', 'number'], [i, ptrY, ptrX]);

		const y = Module.HEAP32[ptrY >> 2];
		const x = Module.HEAP32[ptrX >> 2];

		cellOrder.push({ y, x });

		Module._free(ptrY);
		Module._free(ptrX);
	}

	return cellOrder;
}

export function updateMaze(isGeneration) {
	const interval = isGeneration ? getGeneratorSpeedInterval() : getSolverSpeedInterval();
	const cellOrder = getCellOrder();
	let index = 0;

	// Show entry and exit before
	for (let y = 1; y < getHeight() - 1; y++) {
		const entryCell = Module.ccall('getTypeCell', 'number', ['number', 'number'], [y, 1]);
		const exitCell = Module.ccall('getTypeCell', 'number', ['number', 'number'], [y, getWidth() - 2]);

		if (entryCell === 3) {
			updateCellClass(y, 1, 0);
		}

		if (exitCell === 4) {
			updateCellClass(y, getWidth() - 2, 0);
		}
	}

	// Update the rest of the maze
	if (interval === 0) {
		cellOrder.forEach(({ y, x }) => {
			updateCellClass(y, x, 0);
		});

		if (isGeneration) {
			setIsGenerating(false);
		} else {
			setIsSolving(false);

			if (getHasPath()) {
				showToast('success', 'Success', 'Maze solved successfully! Path to the exit is available.');
			} else {
				showToast('error', 'Error', 'Maze solution not found. No path exists.');
			}
		}

		setControlsState(true);
		setHasPath(false);
		return;
	} else {
		setUpdateTimer(
			setInterval(() => {
				if (index >= cellOrder.length) {
					clearInterval(getUpdateTimer());
					setUpdateTimer(null);

					if (isGeneration) {
						setIsGenerating(false);
					} else {
						setIsSolving(false);

						if (getHasPath()) {
							showToast('success', 'Success', 'Maze solved successfully! Path to the exit is available.');
						} else {
							showToast('error', 'Error', 'Maze solution not found. No path exists.');
						}
					}

					setControlsState(true);
					setHasPath(false);
					return;
				}

				const { y, x } = cellOrder[index];
				updateCellClass(y, x, 1);
				index++;
			}, interval)
		);
	}

	if (isGeneration) {
		setIsGenerating(true);
	} else {
		setIsSolving(true);
	}

	setControlsState(false);
}
