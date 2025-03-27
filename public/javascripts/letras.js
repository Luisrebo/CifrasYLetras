Module.onRuntimeInitialized = function () {
    console.log("Letrassss")
    /* LETRAS */
    //Solo quiero que se pueda añadir una letra
    document.getElementById("formulario-Letras").addEventListener("submit", function (event) {
        event.preventDefault(); // Evitar recargar la página

        let letras = "";
        for (let i = 1; i <= 10; ++i) {
            let valorInput = document.getElementById("inPutLetra" + i).value;

            if (/^[Ñ]$/.test(valorInput)) 
                //la ñ la trato como } por ser el siguiente ascii de z
                letras +='{';
            /* Devuelve true si es una letra válida */
            else if (/^[a-zñ]$/i.test(valorInput))
                letras += valorInput.toLowerCase();
              
             else 
                // Si no es válida, podemos salir o manejar el error
                return;
            
        }

        try {
            let resultado = Module.resuelveLetras(letras);

            document.getElementById("container-Solucion-Letras").style.display = "block";
            document.getElementById("resultadoLetras").innerText = resultado;

        } catch (e) {
            console.error("Error al llamar a resuelveLetras:", e);
        }
    });


    document.getElementById("btnLetrasAleatorias").addEventListener("click", generarLetrasAleatorias);

    function generarLetrasAleatorias() {
        event.preventDefault();

        const totalInputs = 10;
        const vowels = ["A", "E", "I", "O", "U"];
const consonants = ["B", "C", "D", "F", "G", "H", "J", "K", "L", "M", "N", "Ñ", "P", "Q", "R", "S", "T", "V", "W", "X", "Y", "Z"];
        // Generamos un array con índices de 0 a totalInputs-1
        let indices = Array.from({ length: totalInputs }, (_, i) => i);
        // Mezclamos el array para obtener una selección aleatoria
        indices.sort(() => Math.random() - 0.5);
        // Elegimos los primeros 4 índices para que sean vocales
        let vowelIndices = new Set(indices.slice(0, 4));

        // Creamos un array "letras" para almacenar las letras generadas
        let letras = new Array(totalInputs);
        for (let i = 0; i < totalInputs; i++) {
            if (vowelIndices.has(i)) {
                // Elegir una vocal aleatoria
                let randomIndex = Math.floor(Math.random() * vowels.length);
                letras[i] = vowels[randomIndex];
            } else {
                // Elegir una consonante aleatoria
                let randomIndex = Math.floor(Math.random() * consonants.length);
                letras[i] = consonants[randomIndex];
            }
        }

        // Actualizamos los inputs correspondientes (IDs: inPutLetra1, inPutLetra2, ..., inPutLetra10)
        for (let i = 0; i < totalInputs; i++) {
            document.getElementById("inPutLetra" + (i + 1)).value = letras[i];
        }


    }

    document.getElementById("btnClearResultadoLetras").addEventListener("click", borrarResultadoLetras);
    function borrarResultadoLetras() {
        event.preventDefault();
        document.getElementById("resultadoLetras").textContent ='';
        document.getElementById("container-Solucion-Letras").style.display = "none";
    }
      /* LETRAS */
    // Solo quiero que se pueda añadir una letra en los inputs
    document.getElementById("ContenedorLetras").addEventListener("input", function (event) {
        // Verifica si el evento proviene de un input con la clase "input-letra"
        console.log("Evento input detectado en:", event.target.id); // Esto debe imprimirse al escribir en un input
        if (event.target.classList.contains("input-letra")) {
            console.log("Dentro del input de letras");
            let valor = event.target.value;

            // Mantiene solo la primera letra minúscula y permite la ñ
            event.target.value = valor.toLowerCase().slice(0, 1).replace(/[^a-zñ]/g, "");
        }
    });

    document.getElementById("btnClearLetras").addEventListener("click", borrarLetras);
    function borrarLetras() {
        event.preventDefault();
        for (let i = 1; i <= 10; ++i) {
            document.getElementById("inPutLetra" + i).value = '';
        }
    }

};
