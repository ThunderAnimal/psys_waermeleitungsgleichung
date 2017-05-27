/**
 * Created by insanemac on 24/05/2017.
 */
$( document ).ready(function() {
    //Set UP Range UI
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


    // Handler for .ready() called.
    $("#btnStart").click(function () {
        startCalculation();
    });

    $("#btnRestart").click(function () {
       resetCalculation();
    });

    $("body").on('click', '.heatMap_small .heatMapContainer', function () {
        console.log('onclick');
        const container = $(this);
        $(".heatMap_small .heatMapContainer").each(function(){
            if($(this) != container){
                $(this).removeClass("active");
            }
        });
        container.addClass("active");
    });


});

function QueueHeatMap() {
    const that = this;

    let queue = [];
    let isRunnunig = false;
    let isFinish = false;

    this.addHeatMap = function (heatMap) {
        queue.push(heatMap);
    };

    this.startCalculation = function(){
        isFinish = false;
        isRunnunig = true;
        execQueue();
    };
    this.setEndCalculation = function(){
        isFinish = true;
    };
    this.finishQueue = function () {
        isRunnunig = false;
    };



    const execQueue = function () {

        const helperExecQueue = function (item_pos) {
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
                //Prüfen ob Liste über 20 Heatmaps groß ist dann schritt weiter erhöhen fürs Zeichnen
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
            }, 1000);

        };

        helperExecQueue(0);
    };
}

function setUpHeatMap(rasterSize, container){
    const items = rasterSize * rasterSize;
    const size = 100 / rasterSize + "%";

    container.empty();
    for(let y = 0; y < rasterSize; y++){
        for(let x = 0; x < rasterSize; x++){
            container.append(' <div class="heatMapContainer waves-effect waves-teal" data-x="' + x + '" data-y="' + y + '" data-temperature="" style="height:' + size + ' ;width: ' + size + ';"></div>')
        }
        container.append('<div class="clearfix"></div>');
    }
}

function drawHeatMap(heatMap, item_pos){
    $("#currentStep").text(item_pos + 1);
}

function startCalculation(){
    //SetUp UI
    Materialize.toast('Starte Verabeitung', 1500);
    $("#inputParameters").hide();
    $("#loaderCalcProc").show();
    $("#countSteps").text("Berechne...");
    $("#btnRestart").addClass("disabled");
    $("#result").show();

    const spawn = require('child_process').spawn;
    const exec = spawn('../serial/heat_conduction', ['0.001', '51', '0.10', '2']);
    const queueHeatMap = new QueueHeatMap();

    let heatMap = [];
    let isEnd = false;
    let steps = 0;
    let tempValue = ""; //Daten die buffer abgeschitten sind

    queueHeatMap.startCalculation();

    exec.stdout.on('data', (data) => {
        const array = data.toString().split(',');
        for(let i = 0; i < array.length; i++){
            if(isEnd){ //Am End edie Anzahl der Steps speichern
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

    exec.stderr.on('data', (data) => {
        console.log(`stderr: ${data}`);

    });

    exec.on('close', (code) => {
        console.log(`child process exited with code ${code}`);
        queueHeatMap.setEndCalculation();
        $("#countSteps").text(steps);
        $("#loaderCalcProc").hide();
        $("#btnRestart").removeClass("disabled");
    });
}

function resetCalculation() {
    $("#result").hide();
    $("#inputParameters").show();
}