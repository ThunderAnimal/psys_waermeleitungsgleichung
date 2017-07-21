/**
 * Created by insanemac on 24/05/2017.
 */

//global Queue Object to handle showing HeatMaps
let queueHeatMap = {};

/**
 * Docuement Ready
 */
$( document ).ready(function() {
    //Set UP Range UI Sliders
    let rangeDiffTemperature = document.getElementById('rangeDiffTemperature');
    let inputDiffTemperature = document.getElementById('inputDifftemerature');
    noUiSlider.create(rangeDiffTemperature,
        {
            start: 0.001,
            connect: true,
            range: {
                'min': [0.001],
                '10%': [0.01, 0.01],
                '20%': [0.1, 0.1],
                '50%': [1, 0.5],
                'max': [5]
            },
            format: wNumb({
                decimals: 3,
            })
        }
    );
    rangeDiffTemperature.noUiSlider.on('update', function( values, handle ) {
        inputDiffTemperature.value = values[handle] + ' °C';
    });
    inputDiffTemperature.addEventListener('change', function(){
        let val = this.value.replace('°C', '');

        rangeDiffTemperature.noUiSlider.set(val);
    });

    let rangeWaermeleitfaehigkeit = document.getElementById('rangeWaermeleitfaehigkeit');
    let inputWaermeleifaehigkeit = document.getElementById('inputWaermeleitfaehigkeit');
    noUiSlider.create(rangeWaermeleitfaehigkeit,
        {
            start: 0.5,
            connect: true,
            range: {
                'min': [0.01],
                '10%': [0.1, 0.05],
                '50%': [0.5, 0.1],
                'max': [1]
            },
            format: wNumb({
                decimals: 2,
            })
        }
    );
    rangeWaermeleitfaehigkeit.noUiSlider.on('update', function( values, handle ) {
        inputWaermeleifaehigkeit.value = values[handle];
    });
    inputWaermeleifaehigkeit.addEventListener('change', function(){
        rangeWaermeleitfaehigkeit.noUiSlider.set(this.value);
    });

    let rangeRasterSize = document.getElementById('rangeRasterSize');
    let inputRasterSize = document.getElementById('inputRasterSize');
    noUiSlider.create(rangeRasterSize,
        {
            start: 11,
            step: 2,
            connect: true,
            range: {
                'min': [5],
                'max': [51]
            },
            format: wNumb({
                decimals: 0
            })
        }
    );
    rangeRasterSize.noUiSlider.on('update', function( values, handle ) {
        const val = parseInt(values[handle], 10)
        inputRasterSize.value = val;
        setUpHeatMap(val, $('.heatMap_small'));
    });
    inputRasterSize.addEventListener('change', function(){
        rangeRasterSize.noUiSlider.set(this.value);
    });


    // On Start Click
    $("#btnStart").click(function () {
        startCalculation();
    });

    //On RestStart Click
    $("#btnRestart").click(function () {
       resetCalculation();
    });

    //Click on HeatMap Element
    $("body").on('click', '.heatMap_small .heatMapContainer', function () {
        const container = $(this);
        $(".heatMap_small .heatMapContainer").each(function(){
            if($(this) != container){
                $(this).removeClass("active");
            }
        });
        container.addClass("active");
    }).on('click', '.heatMap .heatMapContainer', function () {
        const container = $(this);
        Materialize.toast('Temperatur: ' + container.data('temperature') + ' °C', 2000);
    });


});

/**
 * HeatPoint Class
 * @param PointX
 * @param PointY
 * @param temperature
 * @constructor
 */
function HeatPoint(PointX, PointY, temperature){
    this.PointX = PointX;
    this.PointY = PointY;
    this.temperature = temperature;
}

/**
 * QueueHeatMap Class
 * @constructor
 *
 * Purpose: Handle queue to draw HeartMaps
 */
function QueueHeatMap() {
    const that = this;

    let queue = [];
    let isRunnunig = false;
    let isFinish = false;

    //Zeitintervall zur Abarbeitung der Heatmap
    let timeIntervalExecQueue = 1000;

    /**
     * Add new Heatmap to Queue
     * @param heatMap
     */
    this.addHeatMap = function (heatMap) {
        const heatMapPoints = [];
        const rasterSize = Math.sqrt(heatMap.length);
        for(let i = 0; i < heatMap.length; i++){
            const y = parseInt((i/rasterSize),10);
            let x = ((i+1) % rasterSize) - 1;
            if(x == -1){
                x = rasterSize - 1
            }
            heatMapPoints.push(new HeatPoint(x,y,heatMap[i]));
        }
        queue.push(heatMapPoints);
    };

    /**
     * Start Calculation to get Heatmaps
     * --> startet Abarbeitung der Queue
     */
    this.startCalculation = function(){
        isFinish = false;
        isRunnunig = true;
        execQueue();
    };

    /**
     * Information that the Calalucaltion for new Heatmaps finished
     */
    this.setEndCalculation = function(){
        isFinish = true;
    };

    /**
     * Force Finishing exec Queue
     */
    this.finishQueue = function () {
        isRunnunig = false;
    };

    /**
     * Abarbeitung der Queue der Heatmaps
     */
    const execQueue = function () {

        //Rekursive Function zur Abarbeitung
        const helperExecQueue = function (item_pos) {
            //Wenn durch neue Verarbeitung abgebrochen
            if(!isRunnunig){
                return;
            }

            //Wenn Queue abgearbeitet und die Verarbeitung beendet ist dann Prcodeure verlassen
            if(isFinish && item_pos == queue.length - 1){
                drawHeatMap(queue[item_pos], item_pos);
                that.finishQueue();
                return;
            }

            //Wenn ungültiges Element dann warten und erneut probieren
            if(!queue[item_pos]){
                setTimeout(function(){
                    helperExecQueue(item_pos);
                }, 500);
                return;
            }

            //HEAT Map Zeichnen
            drawHeatMap(queue[item_pos], item_pos);

            //Alle  Sekunde nur die neue Heatmap anzeigen
            setTimeout(function(){
                //Prüfen ob Liste über 20 Heatmaps groß ist dann schrittweite erhöhen fürs Zeichnen
                if(queue.length > 20){
                    const schrittweite = parseInt(queue.length / 20, 10);
                    if(item_pos + schrittweite < queue.length){
                        helperExecQueue(item_pos + schrittweite);
                    }
                    else{
                        helperExecQueue(queue.length - 1);
                    }
                }else{
                    helperExecQueue(item_pos + 1);
                }
            }, timeIntervalExecQueue);

        };

        helperExecQueue(0);
    };
}

/**
 * Heat Map Raster erstellen
 * @param rasterSize
 * @param container
 */
function setUpHeatMap(rasterSize, container){
    const size = 100 / rasterSize + "%";

    container.empty();
    for(let y = 0; y < rasterSize; y++){
        for(let x = 0; x < rasterSize; x++){
            container.append(' <div id="containerX' + x + 'Y' + y + '" class="heatMapContainer waves-effect waves-teal" data-x="' + x + '" data-y="' + y + '" data-temperature="" style="height:' + size + ' ;width: ' + size + ';"></div>')
        }
        container.append('<div class="clearfix"></div>');
    }
}

/**
 * HeatMap zeichnen
 * @param heatMap
 * @param item_pos
 */
function drawHeatMap(heatMap, item_pos){
    $("#currentStep").text(item_pos + 1);

    //Cala Color from value [1,0]
    //Grundlage ist die HSL Farbscale
    function heatMapColorforValue(value){
        const h = (1.0 - value) * 240;
        return "hsl(" + h + ", 100%, 50%)";
    }
    for(let i = 0; i < heatMap.length; i++){
        const x = heatMap[i].PointX;
        const y = heatMap[i].PointY;

        const container = $(".heatMap #containerX" + x + "Y" + y);
        container.attr("data-temperature", heatMap[i].temperature.toString());
        container.css('background-color', heatMapColorforValue(heatMap[i].temperature/100));

    }
}

/**
 * start Calculation
 */
function startCalculation(){
    //init new Queue
    queueHeatMap = new QueueHeatMap();

    /**
     * get Parameters to start cli programm
     * @returns {*}
     */
    const func_getParameters = function () {
        const startTemperature = 100;
        const cornerTemperature = 0;
        const modus = 2; //GUI Modus
        const rasterSize = parseInt( document.getElementById('rangeRasterSize').noUiSlider.get(), 10);
        const diffEndTemperatre =  document.getElementById('rangeDiffTemperature').noUiSlider.get();
        const waermeleitfaehigkeit =  document.getElementById('rangeWaermeleitfaehigkeit').noUiSlider.get();

        let startPointX = 0;
        let startPointY = 0;

        let found = false;
        $(".heatMap_small .heatMapContainer.active").each(function(){
            found = true;
            startPointX = $(this).data("x");
            startPointY = $(this).data("y");
        });
        if(!found){
            return false;
        }

        return [diffEndTemperatre, rasterSize, waermeleitfaehigkeit, startTemperature, cornerTemperature, startPointX, startPointY, modus];
    };

    /**
     * get Path to exe File
     * @returns {string}
     */
    const func_getExecPath = function(){
        const val = $('input[name="group1"]:checked').val();
        return '../' + val + '/heat_conduction';
    };

    /**
     * Set Up UI on Start
     * @param rasterSize
     */
    const func_setUpUIStart = function(rasterSize){
        //SetUp UI
        Materialize.toast('Starte Verabeitung', 1500);
        $("#inputParameters").hide();
        $("#loaderCalcProc").show();
        $("#countSteps").text("Berechne...");
        $("#btnRestart").addClass("disabled");
        $("#result").show();

        setUpHeatMap(inputParameters[1], $(".heatMap"));

    };

    /**
     * Exec Calculation programm
     * @param execPath
     * @param startUpParameters
     */
    const func_execCalculation = function(execPath ,startUpParameters){
        const startTime = Date.now();
        const spawn = require('child_process').spawn;

        //exec
        const exec = spawn(execPath, startUpParameters);


        let heatMap = [];
        let isEnd = false;
        let steps = 0;
        let tempValue = ""; //Daten die buffer abgeschitten sind

        queueHeatMap.startCalculation();

        //Get Systems Output from programm
        exec.stdout.on('data', (data) => {
            //Aus Systems Output Heatmapdata ermitteln
            const array = data.toString().split(',');
            for(let i = 0; i < array.length; i++){
                if(isEnd){ //Am Ende die Anzahl der Steps speichern
                    steps = array[i];
                    return;
                }
                if(array[i] == "#"){ //Ende der HeatMap
                    queueHeatMap.addHeatMap(heatMap);
                    heatMap = [];
                }else if (array[i] == '##'){//Ende der Verarbeitung
                    isEnd = true;
                }else{
                    let val = array[i];
                    if(i == array.length - 1){ //Wenn letztes Item dann abschnittenen wert merken
                        tempValue = array[i];
                    }else{
                        if(i == 0){ //Wenn erstes Item dann mit abschnitten Wert aus vorherigen Buffer zusammenfuegen
                            val = tempValue + array[i]
                        }
                        else{
                            val = array[i];
                        }
                        if(array[i] != ""){
                            heatMap.push(val)
                        }
                    }
                }
            }
        });

        //On Error Log
        exec.stderr.on('data', (data) => {
            console.log(`stderr: ${data}`);

        });

        //Catch when the calc program finished
        exec.on('close', (code) => {
            const endTime = Date.now();
            console.log(`child process exited with code ${code}`);
            queueHeatMap.setEndCalculation();
            func_setUpUIEnd(steps, endTime - startTime);
        });
    };

    /**
     * Setup UI when calculation finished
     * @param steps
     * @param time
     */
    const func_setUpUIEnd = function(steps, time){
        $("#time").text(time/1000);
        $("#countSteps").text(steps);
        $("#loaderCalcProc").hide();
        $("#btnRestart").removeClass("disabled");
    };


    //Start der Verarbeitung
    const inputParameters = func_getParameters();
    if(!inputParameters){
        Materialize.toast('Wählen Sie einen Startpunkt aus!', 3000);
        return;
    }
    func_setUpUIStart(inputParameters);
    setTimeout(function(){
       func_execCalculation(func_getExecPath(), inputParameters);
    }, 500);
}

/**
 * reset Calculation
 */
function resetCalculation() {
    queueHeatMap.finishQueue();
    $("#result").hide();
    $("#inputParameters").show();
}