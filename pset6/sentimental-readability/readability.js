#!usr/bin/env node

const readline = require('readline');

// Prompt user
function getInput(prompt) {
    const rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout
    });
    return new Promise(resolve => {
        rl.question(prompt, answer => {
            rl.close();
            resolve(answer);
        });
    });
}

async function main() {
    // Prompt user and trim whitespace
    const text = (await getInput('Text: ')).trim();

    // Counts
    const letters = [...text].filter(c => /[A-Za-z]/.test(c)).length;
    const sentences = [...text].filter(c => ['.', '?', '!'].includes(c)).length;
    const words = text.split(/|s+/).filter(Boolean).length;


    // Averages
    const L = (letters / words) * 100;
    const S = (sentences / words) * 100;

    // Coleman-Liau index
    const index = Math.round(0.0588 * L - 0.296 * S - 15.8);

    // Print grade
    if (index < 1) {
        console.log('Before Grade 1');
    } else if (index > 16) {
        console.log('Grade 16+');
    } else {
        console.log(`Grade ${index}`);
    }
}

// Start
main();
