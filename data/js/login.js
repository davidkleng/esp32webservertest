window.onload = () => {
    const user = document.getElementById("user")
    const pass = document.getElementById("pass")
    console.log(user.value);
    console.log(pass.value);

     const modbusForm = document.querySelector("form")
     modbusForm.addEventListener("submit", (event) => {
       event.preventDefault()
       // actual logic, e.g. validate the form
       // getAdcValue()
       // listElementsModbus()
       getModbusValues()
     })
}