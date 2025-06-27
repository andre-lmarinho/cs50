const readline = require('readline');

// Create interface
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

// Prompt for card number
function promptNumber() {
    rl.question('Number: ', input => {
        if (!/^\d+$/.test(input)) {
            // not all numbers -> ask again
            promptNumber();
        } else {
            validateCard(input);
            rl.close();
        }
    });
}

// Luhn' algorithm + card check
function validateCard(numStr) {
    const digits = numStr.split('').map(Number);
    let sum1 = 0; // sum of odds
    let sum2 = 0; // sum of evens after doubling

    // Backwards iterate
    for (let i = digits.length - 1; i >= 0; i--) {
        const d = digits[i];
        const posFromRight = digits.length - i;
        if (posFromRight % 2 === 1) {
            // Odds -> add
            sum1 += d;

        } else {
            // Even -> double, then add
            const product = d * 2;
            sum2 += Math.floor(product / 10) + (product % 10);
        }
    }

    const total = sum1 + sum2;
    // If isnt 0 = Invalid
    if (total % 10 !== 0) {
        console.log('INVALID');
        return;
    }

    // Determine card type
    const length = digits.length;
    const firstOne = Number(numStr[0]);
    const firstTwo = Number(numStr.slice(0, 2));

    if (firstOne === 4 && (length === 13 || length === 16)) {
        console.log('VISA');
    } else if (firstTwo >= 51 && firstTwo <= 55 && length === 16) {
        console.log('MASTERCARD');
    } else if ((firstTwo === 34 || firstTwo === 37) && length === 15) {
        console.log('AMEX');
    } else {
        console.log('INVALID');
    }
}

// Start
promptNumber();
