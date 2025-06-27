document.querySelectorAll('.choice').forEach(button => {
    button.onclick = () => {
        let feedback = document.getElementById('feedback1');

        // Remove classes anteriores
        document.querySelectorAll('.choice').forEach(b => {
            b.classList.remove('correct', 'incorrect');
        });

        if (button.textContent === 'Paris') {
            button.classList.add('correct');
            feedback.textContent = "Correct!";
        } else {
            button.classList.add('incorrect');
            feedback.textContent = "Incorrect";
        }
    };
});

document.getElementById('check-answer').onclick = () => {
    const input = document.getElementById('input-answer');
    const feedback = document.getElementById('feedback2');

        // Limpa estado anterior
    input.classList.remove('correct', 'incorrect');

    if (input.value.trim() === "8") {
        input.classList.add('correct');
        feedback.textContent = "Correct!";
    } else {
        input.classList.add('incorrect');
        feedback.textContent = "Incorrect";
    }
};
