import io.shiftleft.semanticcpg.language._

@main def exec(cpgFile: String) = {
    importCpg(cpgFile)
    cpg.method.name.l
}