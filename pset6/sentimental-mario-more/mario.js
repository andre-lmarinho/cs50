const readline = require('readline');


// Set height bounds
const MAX_HEIGHT = 8;
const MIN_HEIGHT = 1;


// Create readline interface
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});


// Prompt for height until valid input (Between min and max)
function promptHeight() {
    rl.question('Who tall boss: ', answer => {
        const height = parseInt(answer, 10);
        if (isNaN(height) || height < MIN_HEIGHT || height > MAX_HEIGHT) {
            // invalid input -> ask again
            promptHeight();
        } else {
            buildPyramid(height);
            rl.close();
        }
    });
}


// Build and print the pyramid
function buildPyramid(height){
    for (let row = 1; row <= height; row++) {
        const spaces = ' '.repeat(height - row); // Spaces
        const blocks = '#'.repeat(row); //Hashes for row
        console.log(`${spaces}${blocks}  ${blocks}`) //Building with two spaces
    }
}


// Start
promptHeight();
