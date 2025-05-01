Module.onRuntimeInitialized = function () {
   
    /* LETRAS */
    //Solo quiero que se pueda añadir una letra
    document.getElementById("formulario-Letras").addEventListener("submit", function (event) {
        event.preventDefault(); // Evitar recargar la página
    
        let letras = "";
        for (let i = 1; i <= 10; ++i) {
            let valorInput = document.getElementById("inPutLetra" + i).value;
    
            if (/^[Ñ]$/.test(valorInput)) {
                letras += '{';
            } else if (/^[a-zñ]$/i.test(valorInput)) {
                letras += valorInput.toLowerCase();
            } else {
                return;
            }
        }
    
        try {
            let resultado = Module.resuelveLetras(letras);
    
            // 1) Obtenemos la cadena interna (con '{' en lugar de 'Ñ')
            let rawPalabra = resultado.getPalabra();
            // 2) La formateamos con tu función (reemplaza '{'→'Ñ')
            let palabraSol = formatearSolucion(rawPalabra);
            // 3) Calculamos la longitud sobre la cadena ya formateada
            const longitud = palabraSol.length;
    
            console.log("Solución formateada:", palabraSol, "Longitud:", longitud);
    
            // 4) Construimos el texto de salida
            let outputText = "Resultado para las letras:\n(";
            for (let i = 0; i < 10; ++i) {
                let letra = (letras[i] === '{') ? 'Ñ' : letras[i].toUpperCase();
                outputText += letra + " ";
            }
            outputText += `)\nCon longitud ${longitud}:\n${palabraSol}`;
    
            // 5) Lo mostramos en pantalla
            document.getElementById("container-Solucion-Letras").style.display = "block";
            const divResultado = document.getElementById("resultadoLetras");
            divResultado.style.whiteSpace = "pre-wrap";
            divResultado.textContent = outputText;


         // Ahora añadimos el enlace al DLE
         const url = `https://dle.rae.es/${encodeURIComponent(palabraSol)}`;
         const link = document.createElement("a");
         link.href = url;
         link.target = "_blank";
         link.rel = "noopener noreferrer";
         link.textContent = `Ver definición de ${palabraSol}`;
         // Opcional: le añades una clase para estilo
         link.classList.add("enlace-dle");
 
         // Dos saltos antes del enlace
         divResultado.appendChild(document.createElement("br"));
         divResultado.appendChild(document.createElement("br"));
         divResultado.appendChild(link);
 
         document.getElementById("container-Solucion-Letras")
                 .scrollIntoView({ behavior: "smooth" });
 
     } catch (e) {
         console.error("Error al llamar a resuelveLetras:", e);
     }
    });


    document.getElementById("btnLetrasAleatorias").addEventListener("click", generarLetrasAleatorias);

    function generarLetrasAleatorias() {
       //

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
        //
        document.getElementById("resultadoLetras").textContent = '';
        document.getElementById("container-Solucion-Letras").style.display = "none";

        //vuelvo a llevar la vista al formulario de input
        document.getElementById("formulario-Letras").scrollIntoView({ behavior: "smooth" });
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
        //
        for (let i = 1; i <= 10; ++i) {
            document.getElementById("inPutLetra" + i).value = '';
        }
    }

    function formatearSolucion(palabra) {
        let palabraFormateada = "";
        for (let letra of palabra) {
            if (letra === '{')
                letra = 'Ñ';

            palabraFormateada += letra;
        }
        return palabraFormateada;
    }
};
