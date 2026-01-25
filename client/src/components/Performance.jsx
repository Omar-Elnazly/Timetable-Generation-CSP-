import { motion } from "framer-motion";

const Performance = () => {
    return (
        <section
            id="performance"
            className="min-h-screen bg-black flex flex-col items-center justify-center px-4 text-center"
        >
            <motion.div
                initial={{ opacity: 0, y: 50 }}
                whileInView={{ opacity: 1, y: 0 }}
                transition={{ duration: 0.8, ease: "easeOut" }}
                viewport={{ once: true, amount: 0.3 }}
                className="max-w-5xl mx-auto"
            >
                <h2 className="text-5xl sm:text-6xl lg:text-7xl font-bold text-white mb-4">
                    <span className="gradient-text">Metrics</span>
                </h2>
                <p className="text-lg text-gray-400 mb-12 max-w-3xl mx-auto">
                    Powered by an optimized C++ constraint solver.
                </p>
                <div className="grid grid-cols-1 md:grid-cols-2 gap-6 max-w-5xl mx-auto">
                    <div className="p-8 rounded-2xl bg-gradient-to-br from-purple-900/30 to-gray-900 border-2 border-purple-500 hover:border-purple-400 hover:from-purple-800/40 hover:to-gray-800 hover:scale-105 hover:shadow-2xl hover:shadow-purple-500/30 hover:-translate-y-2 transition-all duration-500 group">
                        <h3 className="text-2xl font-bold text-white mb-2 group-hover:text-blue-400 transition-colors duration-500">
                            Algorithm
                        </h3>
                        <div className="text-4xl font-bold text-blue-400 mb-4 group-hover:text-blue-300 transition-colors duration-500">
                            Backtracking + MRV
                        </div>
                        <ul className="text-left text-gray-300 space-y-3 group-hover:text-gray-200 transition-colors duration-500">
                            <li className="flex items-center">
                                <span className="text-blue-400 mr-3 text-xl">✓</span>
                                <span>Forward Checking (Pruning)</span>
                            </li>
                            <li className="flex items-center">
                                <span className="text-blue-400 mr-3 text-xl">✓</span>
                                <span>Minimum Remaining Values Heuristic</span>
                            </li>
                            <li className="flex items-center">
                                <span className="text-blue-400 mr-3 text-xl">✓</span>
                                <span>Hard Constraint Validation</span>
                            </li>
                        </ul>
                    </div>

                    <div className="p-8 rounded-2xl bg-gradient-to-br from-green-900/30 to-gray-900 border-2 border-green-500 hover:border-green-400 hover:from-green-800/40 hover:to-gray-800 hover:scale-105 hover:shadow-2xl hover:shadow-green-500/30 hover:-translate-y-2 transition-all duration-500 group">
                        <h3 className="text-2xl font-bold text-white mb-2 group-hover:text-green-400 transition-colors duration-500">
                            Complexity
                        </h3>
                        <div className="text-4xl font-bold text-green-400 mb-4 group-hover:text-green-300 transition-colors duration-500">
                            O(d^n) Space
                        </div>
                        <ul className="text-left text-gray-300 space-y-3 group-hover:text-gray-200 transition-colors duration-500">
                             <li className="flex items-center">
                                <span className="text-green-400 mr-3 text-xl">✓</span>
                                <span>Variables: 200+ Sessions</span>
                            </li>
                            <li className="flex items-center">
                                <span className="text-green-400 mr-3 text-xl">✓</span>
                                <span>Domain: 1200+ (Timeslots * Rooms)</span>
                            </li>
                            <li className="flex items-center">
                                <span className="text-green-400 mr-3 text-xl">✓</span>
                                <span>Solve Time: ~10s (Avg)</span>
                            </li>
                        </ul>
                    </div>
                </div>
            </motion.div>
        </section>
    );
};

export default Performance;
