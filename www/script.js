function saludar() {
    const nombre = prompt("cómo te llamas?");
    const mensaje = `Hola, ${nombre} Bienvenido al laboratorio de JS `;
    document.getElementById("resultado").innerText=mensaje;
    
}

//sumar números
function sumar(){
    const n1=parseFloat(document.getElementById("num1").value);
    const n2=parseFloat(document.getElementById("num2").value);
    const suma = n1+n2;
    document.getElementById("resultadoSuma").innerText = `Resultado: ${suma}`;
}

//multiplicar números
function multiplicar() {
    const num = parseFloat(document.getElementById("num").value);
    const resultado = document.getElementById("resultadosMultiplicacion");
    let tabla = `
        <table border="1">
            <thead>
                <tr>
                    <th>Multiplicación</th>
                    <th>Resultado</th>
                </tr>
            </thead>
            <tbody>
    `;

    for (let i = 1; i <= 10; i++) {
        tabla += `
            <tr>
                <td>${num} × ${i}</td>
                <td>${num * i}</td>
            </tr>
        `;
    }
    tabla += `
            </tbody>
        </table>
    `;

    resultado.innerHTML = tabla;
}
