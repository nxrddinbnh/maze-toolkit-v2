document.addEventListener('DOMContentLoaded', async () => {
    const mazeDiv = document.getElementById('maze');
    const generatorSelect = document.getElementById('generators');
    const speedGeneratorSelect = document.getElementById('speedGenerator');
    const solverSelect = document.getElementById('solvers');
    const speedSolverSelect = document.getElementById('speedSolver');
    const generateButton = document.querySelector('#generateButton');
    const solveButton = document.querySelector('#solveButton');
    const stopButton = document.querySelector('#stopButton');

    let width = 0;
    let height = 0;
    let updateTimer = null;
    let isGenerating = false;
    let isEditing = false;

    // Initialization of selects
    generatorSelect.selectedIndex = 0;
    speedGeneratorSelect.selectedIndex = 1;
    solverSelect.selectedIndex = 0;
    speedSolverSelect.selectedIndex = 1;

    // Create the maze cells
    {
        const cellSize = 30;
        const mazeRect = mazeDiv.getBoundingClientRect();
        const tempWidth = Math.floor(mazeRect.width / cellSize);
        const temHeight = Math.floor(mazeRect.height / cellSize);
        width = tempWidth % 2 === 0 ? tempWidth - 1 : tempWidth;
        height = temHeight % 2 === 0 ? temHeight - 1 : temHeight;

        mazeDiv.style.gridTemplateRows = `repeat(${height}, ${cellSize}px)`;
        mazeDiv.style.gridTemplateColumns = `repeat(${width}, ${cellSize}px)`;
        mazeDiv.innerHTML = "";

        for (let y = 0; y < height; y++) {
            for (let x = 0; x < width; x++) {
                const cell = document.createElement('button');
                cell.classList.add('cell');
                cell.id = `cell-${y}-${x}`;

                if (x === 0 || x === width - 1 || y === 0 || y === height - 1) {
                    cell.classList.add('wall');
                }

                mazeDiv.appendChild(cell);
            }
        }
    }

    function onResize() {
        location.reload();
    }

    function getGeneratorAlgo() {
        return generatorSelect.value;
    }

    function getSolverAlgo() {
        return solverSelect.value;
    }

    function getSpeedInterval(select) {
        switch (select.value) {
            case '1' : return 0;
            case '2' : return 5;
            case '3' : return 10;
            case '4' : return 20;
            case '5' : return 40;
            case '6' : return 200;
            default: return 5;
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

    function setControlsState(isEnabled) {
        generatorSelect.disabled = !isEnabled;
        speedGeneratorSelect.disabled = !isEnabled;
        solverSelect.disabled = !isEnabled;
        speedSolverSelect.disabled = !isEnabled;
        generateButton.disabled = !isEnabled;
        solveButton.disabled = !isEnabled;

        document.body.classList.toggle('not-allowed', !isEnabled);
    }

    // Initialize the maze display
    function prepareMaze() {
        let startType = getGeneratorAlgo();

        if (startType == 1 || startType == 3 || startType == 5 || startType == 6) {
            startType = 3;
        } else if (startType == 2) {
            startType = 2;
        } else {
            startType = 1;
        }
            
        Module.ccall('prepareMaze', null, ['number'], [startType]);

        for (let y = 1; y < height - 1; y++) {
            for (let x = 1; x < width - 1; x++) {
                const cell = document.getElementById(`cell-${y}-${x}`);
                const cellType = Module.ccall('getTypeCell', 'number', ['number', 'number'], [y, x]);

                if (cell && cellType === -1) {
                    cell.className = "";
                    cell.classList.add('cell', 'wall');
                } else if (cell && cellType === 0) {
                    cell.className = "";
                    cell.classList.add('cell');
                }
            }
        }
    }

    function updateMaze() {
        const interval = getSpeedInterval(speedGeneratorSelect);
        const cellOrder = getCellOrder();
        let index = 0;

        if (interval === 0) {
            cellOrder.forEach(({ y, x }) => {
                const cell = document.getElementById(`cell-${y}-${x}`);
                const cellType = Module.ccall('getTypeCell', 'number', ['number', 'number'], [y, x]);

                if (cell && cellType === -1) {
                    cell.className = "";
                    cell.classList.add('cell', 'wall');
                } else if (cell && cellType === 0) {
                    cell.className = "";
                    cell.classList.add('cell');
                }
            });

            isGenerating = false;
            setControlsState(true);
            return;
        } else {
            updateTimer = setInterval(() => {
                if (index >= cellOrder.length) {
                    clearInterval(updateTimer);
                    updateTimer = null;
                    isGenerating = false;
                    setControlsState(true);
                    return;
                }
    
                const { y, x } = cellOrder[index];
                const cell = document.getElementById(`cell-${y}-${x}`);
                const cellType = Module.ccall('getTypeCell', 'number', ['number', 'number'], [y, x]);
    
                if (cell && cellType === -1) {
                    cell.className = "";
                    cell.classList.add('wallAnimation');
                    cell.classList.add('cell', 'wall');
                } else if (cell && cellType === 0) {
                    cell.className = "";
                    cell.classList.add('cell');
                }

                index++;
            }, interval);
        }

        isGenerating = true;
        setControlsState(false);
    }

    // function toggleCellType(y, x) {
    //     const cell = document.getElementById(`cell-${y}-${x}`);
    //     const cellType = Module.ccall('getTypeCell', 'number', ['number', 'number'], [y, x]);
    //     generatorSelect.selectedIndex = 6;

    //     if (cell && cellType === -1) {
    //         Module.ccall('setTypeCell', null, ['number', 'number', 'number'], [y, x, 0]);
    //         cell.className = "";
    //         cell.classList.add('cell');
    //     } else if (cell && cellType === 0) {
    //         Module.ccall('setTypeCell', null, ['number', 'number', 'number'], [y, x, -1]);
    //         cell.className = "";
    //         cell.classList.add('wallAnimation');
    //         cell.classList.add('cell', 'wall');
    //     }
    // }

    // mazeDiv.addEventListener('click', (event) => {
    //     if (event.target && event.target.classList.contains('cell')) {
    //         const cellId = event.target.id;
    //         const [_, y, x] = cellId.split('-').map(Number);
    //         toggleCellType(y, x);
    //     }
    // });

    generateButton.addEventListener('click', () => {
        if (!isGenerating) {
            prepareMaze();

            Module.ccall('generateMaze', 
                null,
                ['number', 'number', 'number'],
                [width, height, getGeneratorAlgo()]
            );

            updateMaze();
        }
    });

    // solveButton.addEventListener('click', () => {
    //     if (!isGenerating) {
    //         Module.ccall('solveMaze', null, ['number'], [getSolverAlgo()]);

    //         setTimeout(updateMaze, 100);
    //     }
    // });

    stopButton.addEventListener('click', () => {
        if (updateTimer) {
            clearInterval(updateTimer);
            updateTimer = null;
            isGenerating = false;
            setControlsState(true);
        }
    });

    window.addEventListener('resize', onResize);
});
