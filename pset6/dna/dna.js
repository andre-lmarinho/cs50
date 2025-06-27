#!/usr/bin/env node

const fs = require('fs');



function main() {
    const args = process.argv;
    // Check command-line usage
    if (args.length !== 4) {
        console.log('Usage: node dna.js data.csv sequence.txt');
        process.exit(1);
    }

    const [, , databaseFile, sequenceFile] = args;

    // Read and parse CSV into array
    const csvData = fs.readFileSync(databaseFile, 'utf8').trim();
    const lines = csvData.split('\n');
    const headers = lines[0].split(',');
    const strNames = headers.slice(1); // Skip name column

    const database = lines.slice(1).map(line => {
        const values = line.split(',');
        const record = {};
        headers.forEach((header, i) => {
            record[header] = values[i];
        });
        return record;
    });

    // Read DNA sequence file into string
    const dnaSequence = fs.readFileSync(sequenceFile, 'utf8').trim();


    // Find Longest match for every STR
    const strCounts = {};
    strNames.forEach(str => {
        strCounts[str] = longestMatch(dnaSequence, str);
    });

    // Check person in DB
    for (const person of database) {
        const isMatch = strNames.every(str =>
            parseInt(person[str], 10) === strCounts[str]
        );
        if (isMatch) {
            console.log(person.name);
            return; // Found > Exit
        }
    }

    // If not Found
    console.log('No match');
}


// Return Length of longest run of subsequence in sequence
function longestMatch(sequence, subsequence) {
    let longestRun = 0;
    const subLen = subsequence.length;
    const seqLen = sequence.length;

    // Check each position in sequence
    for (let i = 0; i < seqLen; i++) {
        let count = 0;

        // Count consecutive repeats
        while (sequence.substr(i + count * subLen, subLen) === subsequence) {
            count++;
        }

        // Update max length
        if (count > longestRun) {
            longestRun = count;
        }
    }

    return longestRun;
}

// Start
main();
