import java.io.FileOutputStream;
import java.lang.instrument.ClassFileTransformer;
import java.lang.instrument.Instrumentation;
import java.security.ProtectionDomain;

public class DumpAgent {
  public static void premain(String args, Instrumentation inst) {
    inst.addTransformer(
        new ClassFileTransformer() {
          @Override
          public byte[] transform(
              Module module,
              ClassLoader loader,
              String className,
              Class<?> classBeingRedefined,
              ProtectionDomain protectionDomain,
              byte[] classfileBuffer) {
            if ("MyRobot".equals(className)) {
              try (FileOutputStream fos = new FileOutputStream("MyRobot.class")) {
                fos.write(classfileBuffer);
              } catch (Exception e) {
                e.printStackTrace();
              }
            }
            return null;
          }
        },
        false);
  }
}
