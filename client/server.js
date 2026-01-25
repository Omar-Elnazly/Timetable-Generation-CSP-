import express from 'express';
import { spawn } from 'child_process';
import cors from 'cors';
import path from 'path';
import fs from 'fs';
import { fileURLToPath } from 'url';
import { dirname } from 'path';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const app = express();
const PORT = 3001;

app.use(cors());
app.use(express.json());

app.post('/api/run-solver', (req, res) => {
    console.log('Running CSP Solver...');
    
    const cppExecutable = path.join(__dirname, '../build/TestSql');
    const jsonOutputPath = path.join(__dirname, '../data/timetable.json');
    
    if (!fs.existsSync(cppExecutable)) {
        return res.status(500).json({ 
            error: 'Executable not found. Please build the project first.',
            path: cppExecutable 
        });
    }

    const process = spawn(cppExecutable, [], {
        cwd: path.join(__dirname, '../build'),
        shell: '/bin/bash'
    });

    let output = '';
    let errorOutput = '';
    const startTime = Date.now();

    process.stdout.on('data', (data) => {
        output += data.toString();
        console.log('Output:', data.toString());
    });

    process.stderr.on('data', (data) => {
        errorOutput += data.toString();
        console.error('Error:', data.toString());
    });

    process.on('close', (code) => {
        const solveTime = ((Date.now() - startTime) / 1000).toFixed(2);
        
        if (code !== 0) {
            console.error('Solver exited with code:', code);
            return res.status(500).json({ 
                error: 'Solver failed', 
                details: errorOutput,
                code: code 
            });
        }

        console.log('Solver completed in', solveTime, 'seconds');
        
        try {
            const jsonData = fs.readFileSync(jsonOutputPath, 'utf8');
            const result = JSON.parse(jsonData);
            
            res.json({
                success: result.success,
                data: {
                    schedule: result.schedule || {},
                    stats: result.stats || {}
                },
                rawOutput: output
            });
        } catch (err) {
            console.error('Failed to read JSON output:', err);
            res.status(500).json({ 
                error: 'Failed to parse solver output', 
                details: err.message 
            });
        }
    });

    process.on('error', (error) => {
        console.error('Failed to start solver:', error);
        res.status(500).json({ 
            error: 'Failed to start solver', 
            details: error.message 
        });
    });
});

app.listen(PORT, () => {
    console.log(`Backend server running on http://localhost:${PORT}`);
    console.log('Executable path:', path.join(__dirname, '../build/TestSql'));
});
