import { motion } from "framer-motion";

const Plans = () => {
    return (
        <section
            id="plans"
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
                    <span className="gradient-text">Performance</span>
                </h2>
                <p className="text-lg text-gray-400 mb-12 max-w-3xl mx-auto">
                    Real-world metrics from production builds. Benchmarked on Arch Linux with GCC 15.2.1, 
                    demonstrating the power of Release mode optimization and intelligent algorithm design.
                </p>
                <div className="grid grid-cols-1 md:grid-cols-2 gap-6 max-w-5xl mx-auto">
                    <div className="p-8 rounded-2xl bg-gradient-to-br from-purple-900/30 to-gray-900 border-2 border-purple-500 hover:border-purple-400 hover:from-purple-800/40 hover:to-gray-800 hover:scale-105 hover:shadow-2xl hover:shadow-purple-500/30 hover:-translate-y-2 transition-all duration-500 group">
                        <h3 className="text-2xl font-bold text-white mb-2 group-hover:text-blue-400 transition-colors duration-500">
                            Scalability
                        </h3>
                        <div className="text-5xl font-bold text-blue-400 mb-4 group-hover:text-blue-300 transition-colors duration-500">
                            All Years
                        </div>
                        <ul className="text-left text-gray-300 space-y-3 group-hover:text-gray-200 transition-colors duration-500">
                            <li className="flex items-center">
                                <span className="text-blue-400 mr-3 text-xl">✓</span>
                                <span>Multi-year support</span>
                            </li>
                            <li className="flex items-center">
                                <span className="text-blue-400 mr-3 text-xl">✓</span>
                                <span>4 specializations (AID, BIF, CSC, CNC)</span>
                            </li>
                            <li className="flex items-center">
                                <span className="text-blue-400 mr-3 text-xl">✓</span>
                                <span>Multiple groups per year</span>
                            </li>
                            <li className="flex items-center">
                                <span className="text-blue-400 mr-3 text-xl">✓</span>
                                <span>Extensible architecture</span>
                            </li>
                        </ul>
                    </div>

                    <div className="p-8 rounded-2xl bg-gradient-to-br from-green-900/30 to-gray-900 border-2 border-green-500 hover:border-green-400 hover:from-green-800/40 hover:to-gray-800 hover:scale-105 hover:shadow-2xl hover:shadow-green-500/30 hover:-translate-y-2 transition-all duration-500 group">
                        <h3 className="text-2xl font-bold text-white mb-2 group-hover:text-green-400 transition-colors duration-500">
                            Optimization Boost
                        </h3>
                        <div className="text-5xl font-bold text-green-400 mb-4 group-hover:text-green-300 transition-colors duration-500">
                            8.3x
                        </div>
                        <ul className="text-left text-gray-300 space-y-3 group-hover:text-gray-200 transition-colors duration-500">
                            <li className="flex items-center">
                                <span className="text-green-400 mr-3 text-xl">✓</span>
                                <span>Release vs Debug mode</span>
                            </li>
                            <li className="flex items-center">
                                <span className="text-green-400 mr-3 text-xl">✓</span>
                                <span>-O3 compiler optimization</span>
                            </li>
                            <li className="flex items-center">
                                <span className="text-green-400 mr-3 text-xl">✓</span>
                                <span>40.69s → 4.87s reduction</span>
                            </li>
                            <li className="flex items-center">
                                <span className="text-green-400 mr-3 text-xl">✓</span>
                                <span>Production-ready speed</span>
                            </li>
                        </ul>
                    </div>
                </div>
            </motion.div>
        </section>
    );
};

export default Plans;
