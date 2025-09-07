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
            scrollToIfMobile(document.getElementById("container-Solucion-Letras"));


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
 
         //document.getElementById("container-Solucion-Letras")
                 //.scrollIntoView({ behavior: "smooth" });
 
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
        //document.getElementById("formulario-Letras").scrollIntoView({ behavior: "smooth" });
    }
    

    // Autoavance, retroceso y pegado en inputs de LETRAS
(function () {
  const N = 10;
  const inputs = Array.from({ length: N }, (_, i) => document.getElementById(`inPutLetra${i + 1}`));

  const oneChar = (v) => {
    if (!v) return "";
    const s = v.normalize("NFD").replace(/[\u0300-\u036f]/g, "");
    const m = s.match(/[a-zA-ZñÑ]/);
    return m ? m[0].toUpperCase() : "";
  };

  inputs.forEach((el, i) => {
    el.addEventListener("focus", () => el.select());

    el.addEventListener("input", () => {
      el.value = oneChar(el.value);
      if (el.value && i < N - 1) inputs[i + 1].focus();
    });

    el.addEventListener("keydown", (e) => {
      if ((e.key === "Backspace" || e.key === "Delete") && !el.value && i > 0) {
        inputs[i - 1].focus();
      } else if (e.key === "ArrowLeft" && i > 0) {
        e.preventDefault(); inputs[i - 1].focus();
      } else if (e.key === "ArrowRight" && i < N - 1) {
        e.preventDefault(); inputs[i + 1].focus();
      }
    });

    el.addEventListener("paste", (e) => {
      e.preventDefault();
      const raw = (e.clipboardData || window.clipboardData).getData("text") || "";
      const norm = raw.normalize("NFD").replace(/[\u0300-\u036f]/g, "");
      const chars = Array.from(norm).filter(ch => /[a-zA-ZñÑ]/.test(ch)).map(ch => ch.toUpperCase());
      if (!chars.length) return;

      let j = i;
      for (const ch of chars) {
        if (j >= N) break;
        inputs[j].value = ch;
        j++;
      }
      (j < N ? inputs[j] : inputs[N - 1]).focus();
    });
  });
})();


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

    function scrollToIfMobile(el) {
  const isSmall = window.matchMedia("(max-width: 768px)").matches;
  if (!isSmall || !el) return;

  const vh = window.innerHeight || document.documentElement.clientHeight;
  const r = el.getBoundingClientRect();
  const notFullyVisible = r.top < 0 || r.bottom > vh;

  if (notFullyVisible) {
    el.scrollIntoView({ behavior: "smooth", block: "start" });
  }
}

};
