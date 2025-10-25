import { motion } from "framer-motion";
import { useState } from "react";

const GetStarted = () => {
    const [loading, setLoading] = useState(false);
    const [error, setError] = useState(null);
    const [output, setOutput] = useState("");
    const [scheduleData, setScheduleData] = useState(null);
    const [selectedYear, setSelectedYear] = useState(1);
    const [selectedGroup, setSelectedGroup] = useState("G1");

    const runSolver = async () => {
        setLoading(true);
        setError(null);
        setOutput("Connecting to backend...\n");
        setScheduleData(null);

        try {
            const response = await fetch('http://localhost:3001/api/run-solver', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
            });

            if (!response.ok) {
                throw new Error(`Server error: ${response.status}`);
            }

            const result = await response.json();
            
            console.log('API Response:', result);
            console.log('Schedule Data:', result.data?.schedule);
            
            if (result.success) {
                setOutput(result.rawOutput || "Solver completed successfully!");
                setScheduleData(result.data);
                console.log('Sessions loaded:', result.data?.schedule);
            } else {
                throw new Error(result.error || 'Unknown error');
            }
        } catch (err) {
            setError(err.message);
            setOutput(`❌ Error: ${err.message}\n\nMake sure the backend server is running:\nnpm run server`);
        } finally {
            setLoading(false);
        }
    };

    const getGroupsForYear = (year) => {
        if (year <= 2) return ["G1", "G2", "G3"];
        else return ["AID", "BIF", "CSC", "CNC"];
        return [];
    };

    const getSessions = () => {
        if (!scheduleData || !scheduleData.schedule) {
            console.log('No schedule data available');
            return [];
        }
        const yearData = scheduleData.schedule[selectedYear];
        if (!yearData) {
            console.log(`No data for year ${selectedYear}`);
            return [];
        }
        const sessions = yearData[selectedGroup] || [];
        console.log(`Year ${selectedYear}, Group ${selectedGroup}:`, sessions);
        return sessions;
    };

    const sessions = getSessions();
    const stats = scheduleData?.stats || null;
    
    const handleReset = () => {
        setScheduleData(null);
        setOutput("");
        setError(null);
        setSelectedYear(1);
        setSelectedGroup("G1");
    };

    return (
        <section
            id="getstarted"
            className="min-h-screen bg-black flex flex-col items-center justify-center px-4 py-20"
        >
            <motion.div
                initial={{ opacity: 0, y: 50 }}
                whileInView={{ opacity: 1, y: 0 }}
                transition={{ duration: 0.8, ease: "easeOut" }}
                viewport={{ once: true, amount: 0.3 }}
                className="max-w-6xl mx-auto w-full"
            >
                <h2 className="text-5xl sm:text-6xl lg:text-7xl font-bold text-white mb-4 text-center">
                    <span className="gradient-text">Get Started</span>
                </h2>
                <p className="text-lg text-gray-400 mb-12 text-center max-w-3xl mx-auto">
                    Run the CSP solver and visualize the generated timetable with zero conflicts.
                </p>

                <div className="bg-gray-900 border border-gray-800 rounded-xl p-6 mb-6">
                    <div className="flex items-center justify-between mb-4">
                        <h3 className="text-xl font-bold text-white">Solver Output</h3>
                        <button
                            onClick={runSolver}
                            disabled={loading}
                            className={`px-6 py-2 ${loading ? 'bg-gray-600' : 'bg-green-600 hover:bg-green-500'} text-white rounded-lg font-semibold transition-all duration-300 hover:scale-105 disabled:cursor-not-allowed`}
                        >
                            {loading ? '⏳ Running...' : '▶ Run Solver'}
                        </button>
                    </div>
                    <div className="bg-black rounded-lg p-4 font-mono text-sm text-green-400 h-64 overflow-y-auto">
                        {output ? (
                            <pre className="whitespace-pre-wrap">{output}</pre>
                        ) : (
                            <div className="text-gray-600 text-center mt-20">
                                Click "Run Solver" to execute the C++ CSP solver...
                            </div>
                        )}
                    </div>
                </div>

                {scheduleData && (
                    <div className="bg-gray-900 border border-gray-800 rounded-xl p-6">
                        <div className="flex gap-4 mb-6">
                            <div>
                                <label className="text-gray-400 text-sm mb-2 block">Select Year:</label>
                                <select
                                    value={selectedYear}
                                    onChange={(e) => {
                                        const year = parseInt(e.target.value);
                                        setSelectedYear(year);
                                        const groups = getGroupsForYear(year);
                                        setSelectedGroup(groups[0]);
                                    }}
                                    className="bg-gray-800 text-white px-4 py-2 rounded-lg border border-gray-700"
                                >
                                    <option value={1}>Year 1</option>
                                    <option value={2}>Year 2</option>
                                    <option value={3}>Year 3</option>
                                    <option value={4}>Year 4</option>
                                </select>
                            </div>
                            <div>
                                <label className="text-gray-400 text-sm mb-2 block">Select Group:</label>
                                <select
                                    value={selectedGroup}
                                    onChange={(e) => setSelectedGroup(e.target.value)}
                                    className="bg-gray-800 text-white px-4 py-2 rounded-lg border border-gray-700"
                                >
                                    {getGroupsForYear(selectedYear).map(group => (
                                        <option key={group} value={group}>{group}</option>
                                    ))}
                                </select>
                            </div>
                        </div>

                        {sessions.length > 0 ? (
                            <div>
                                <div className="overflow-x-auto">
                                    <table className="w-full border-collapse">
                                        <thead>
                                            <tr className="bg-gray-800">
                                                <th className="border border-gray-700 p-3 text-white font-bold">Time / Day</th>
                                                <th className="border border-gray-700 p-3 text-white font-bold">Sunday</th>
                                                <th className="border border-gray-700 p-3 text-white font-bold">Monday</th>
                                                <th className="border border-gray-700 p-3 text-white font-bold">Tuesday</th>
                                                <th className="border border-gray-700 p-3 text-white font-bold">Wednesday</th>
                                                <th className="border border-gray-700 p-3 text-white font-bold">Thursday</th>
                                            </tr>
                                        </thead>
                                        <tbody>
                                            {['09:00AM - 10:30AM', '10:45AM - 12:15PM', '12:30PM - 02:00PM', '02:15PM - 03:45PM'].map(timeSlot => (
                                                <tr key={timeSlot}>
                                                    <td className="border border-gray-700 p-3 bg-gray-800 text-white font-semibold text-center">
                                                        {timeSlot}
                                                    </td>
                                                    {['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday'].map(day => {
                                                        const session = sessions.find(s => {
                                                            const dayMatch = s.day === day;
                                                            const timeMatch = s.time === timeSlot;
                                                            if (dayMatch && timeMatch) {
                                                                console.log(`✅ Match found: ${s.code} at ${day} ${timeSlot}`);
                                                            }
                                                            return dayMatch && timeMatch;
                                                        });
                                                        
                                                        return (
                                                            <td key={day} className="border border-gray-700 p-2">
                                                                {session ? (
                                                                    <div className="bg-gradient-to-br from-green-600/20 to-blue-600/20 border border-green-500/30 rounded p-2 hover:border-green-500 transition-all">
                                                                        <div className="text-white font-bold text-sm">{session.code}</div>
                                                                        <div className="text-gray-300 text-xs mt-1">{session.name}</div>
                                                                        <div className="text-gray-400 text-xs mt-1">
                                                                            <div>{session.instructor}</div>
                                                                            <div>{session.room}</div>
                                                                        </div>
                                                                    </div>
                                                                ) : (
                                                                    <div className="text-gray-600 text-center text-sm">-</div>
                                                                )}
                                                            </td>
                                                        );
                                                    })}
                                                </tr>
                                            ))}
                                            <tr className="bg-purple-900/20">
                                                <td className="border border-gray-700 p-3 bg-purple-800 text-white font-semibold text-center">
                                                    Full Day<br/><span className="text-xs text-gray-300">09:00AM - 03:45PM</span>
                                                </td>
                                                {['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday'].map(day => {
                                                    const session = sessions.find(s => 
                                                        s.day === day && s.time === '09:00AM - 03:45PM'
                                                    );
                                                    
                                                    return (
                                                        <td key={day} className="border border-gray-700 p-2">
                                                            {session ? (
                                                                <div className="bg-gradient-to-br from-purple-600/30 to-pink-600/30 border border-purple-500/50 rounded p-2 hover:border-purple-500 transition-all">
                                                                    <div className="text-white font-bold text-sm">{session.code}</div>
                                                                    <div className="text-gray-300 text-xs mt-1">{session.name}</div>
                                                                    <div className="text-gray-400 text-xs mt-1">
                                                                        <div>{session.instructor}</div>
                                                                        <div>{session.room}</div>
                                                                    </div>
                                                                    <div className="text-purple-300 text-xs mt-1 font-semibold">Full Day</div>
                                                                </div>
                                                            ) : (
                                                                <div className="text-gray-600 text-center text-sm">-</div>
                                                            )}
                                                        </td>
                                                    );
                                                })}
                                            </tr>
                                        </tbody>
                                    </table>
                                </div>
                            </div>
                        ) : (
                            <div className="text-gray-600 text-center py-8">
                                No sessions found for Year {selectedYear} - {selectedGroup}
                            </div>
                        )}
                    </div>
                )}
            </motion.div>
        </section>
    );
};

export default GetStarted;
