Module.onRuntimeInitialized = function () {
  
    document.getElementById("cifrasButton").addEventListener("click", reedirctCifras);

    function reedirctCifras() {
        window.location.href = "./cifras.html";
    }

    document.getElementById("letrasButton").addEventListener("click", reedirctLetras);

    function reedirctLetras() {
        window.location.href = "./letras.html";
    }
};