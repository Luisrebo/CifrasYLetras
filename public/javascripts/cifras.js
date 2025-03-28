Module.onRuntimeInitialized = function () {
    console.log("Emscripten module loaded LUIS");
    console.log("segundo");

    document.getElementById("formulario-Cifras").addEventListener("submit", function (event) {
        event.preventDefault(); // Evitar recargar la página

        let vectorCifras = [];
        for (let i = 1; i < 7; ++i) {

            let valorInput = parseInt(document.getElementById("inPutCifra" + i).value);

            /* isNaN devuelve true si No es un numero (is not a number) aun asi el tipo del input es number en html */
            if (!isNaN(valorInput))
                vectorCifras.push(valorInput);
            /* no puede haber ningun input vacio o sin cifras tamb lo pongo require en el html */
            else
                return;
        }

        // Recoger el número objetivo
        let numObjetivo = parseInt(document.getElementById("inPutNumeroObjetivo").value);
        if (isNaN(numObjetivo)) {
            console.error("Número objetivo no es válido");
            return;
        }

        // Creamos el vector para enviar a C++:
        let vectorInput = new Module.VectorInt();
        // Agregamos las cifras
        for (let i = 0; i < vectorCifras.length; i++) {
            vectorInput.push_back(vectorCifras[i]);
        }
        // Agregamos el número objetivo como el último elemento del vector 
        vectorInput.push_back(numObjetivo);

        // Llamamos a la función exportada (en este ejemplo, 'resuelveCasoCifras')
        try {
            let resultado = Module.resuelveCifras("BFS", vectorInput);
            document.getElementById("container-Solucion-Cifras").style.display = "block";
            document.getElementById("resultado").innerText = resultado;

            //movemos la vista hasta el div que estaba oculto
            document.getElementById("container-Solucion-Cifras").scrollIntoView({behavior:"smooth"});

        } catch (e) {
            console.error("Error al llamar a resuelveCifras:", e);
        }
    });

    document.getElementById("btnAleatorios").addEventListener("click", generarAleatorios);

    function generarAleatorios() {
        event.preventDefault();
        // Posibles cifras
        const posiblesCifras = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 25, 50, 75, 100];

        // Rellenar las 6 cifras
        for (let i = 1; i <= 6; i++) {
            // Escoge un índice aleatorio
            let indiceAleatorio = Math.floor(Math.random() * posiblesCifras.length);
            // Asigna el valor al input correspondiente
            document.getElementById("inPutCifra" + i).value = posiblesCifras[indiceAleatorio];
        }

        // Generar número objetivo aleatorio entre 101 y 999
        let numeroObjetivo = 101 + Math.floor(Math.random() * 899);
        document.getElementById("inPutNumeroObjetivo").value = numeroObjetivo;

        //y borramos el resultado anterior:
        borrarResultado();
    }

    document.getElementById("btnClear").addEventListener("click", borrarCifras);
    function borrarCifras() {
        event.preventDefault();
        for (let i = 1; i <= 6; ++i) {
            document.getElementById("inPutCifra" + i).value = '';
        }
        document.getElementById("inPutNumeroObjetivo").value = '';
    }
    document.getElementById("btnClearResultado").addEventListener("click", borrarResultado);
    function borrarResultado() {
        event.preventDefault();
        document.getElementById("resultado").textContent = '';
        document.getElementById("container-Solucion-Cifras").style.display = "none";
    }
};
